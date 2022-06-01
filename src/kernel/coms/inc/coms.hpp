/**
 * @file coms.hpp
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains functions to work with COM Port (USB send)
 * @version 1.0
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#ifndef KERNEL_COMS_HEADER
#define KERNEL_COMS_HEADER

#include "abstracts.hpp"

#include "../../common.hpp"

/**
 * @brief All COM Port readers
 */
namespace COMS
{
    /**
     * @brief USB Device reader
     */
    class USB :
        virtual public COMS::Abstracts::COM_Abstract
    {
        private:
            /**
             * @brief Configure the Serial Port
             */
            void configure_sp()
            {
                if(tcgetattr(m_serial_port, &m_tty) != 0)
                    throw std::invalid_argument("Error while configuring Serial Port");

                /* Set Baud Rate */
                cfsetospeed (&m_tty, (speed_t)B9600);
                cfsetispeed (&m_tty, (speed_t)B9600);

                /* Setting other Port Stuff */
                m_tty.c_cflag     &=  ~PARENB;        // Make 8n1
                m_tty.c_cflag     &=  ~CSTOPB;
                m_tty.c_cflag     &=  ~CSIZE;
                m_tty.c_cflag     |=  CS8;

                m_tty.c_cflag     &=  ~CRTSCTS;       // no flow control
                m_tty.c_cc[VMIN]   =  1;              // read doesn't block
                m_tty.c_cc[VTIME]  =  5;              // 0.5 seconds read timeout
                m_tty.c_cflag     |=  CREAD | CLOCAL; // turn on READ & ignore ctrl lines

                /* Make raw */
                cfmakeraw(&m_tty);

                /* Flush Port, then applies attributes */
                tcflush(m_serial_port, TCIFLUSH);
                if (tcsetattr (m_serial_port, TCSANOW, &m_tty) != 0) 
                    // TODO: Throw error
                    throw std::invalid_argument(
                        "Error " + (string)strerror(errno) + " from tcsetattr"
                    );
            }

            /**
             * @brief Connect to Serial port
             */
            void connect()
            {
                m_serial_port = open(m_com_dev.c_str(), O_RDWR);
                if (m_serial_port < 0)
                    // TODO: Throw error
                    throw std::invalid_argument(m_com_dev + " - No device found!");
            }

        public:
            /**
             * @brief Construct a new USB object
             * 
             * @param com_dev COM Device path (e.g. /dev/ttyACM1)
             */
            USB(
                string& com_dev
            ) : COMS::Abstracts::COM_Abstract(
                    com_dev
                )
            { }

            /**
             * @brief Destroy the USB object
             */
            virtual ~USB() = default;

            /**
             * @brief Test connection with device
             * 
             * @return true 
             * @return false 
             */
            bool test_connection()
            {
                m_serial_port = open(m_com_dev.c_str(), O_RDWR);

                if (m_serial_port < 0)
                    return false;

                close(m_serial_port);

                return true;
            }
    };

    /**
     * @brief USART Device reader
     * 
     * @note https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
     * @note https://stackoverflow.com/questions/37944461/linux-reading-data-from-uart
     */
    class USART : 
        virtual public COMS::Abstracts::COM_Abstract
    {
        private:
            /**
             * @brief Configure the Serial Port
             */
            void configure_sp()
            {
                if(tcgetattr(m_serial_port, &m_tty) != 0)
                    throw std::invalid_argument("Error while configuring Serial Port");

                cfsetispeed(&m_tty, B115200); /* Set Read  Speed as 19200                       */
                cfsetospeed(&m_tty, B115200); /* Set Write Speed as 19200                       */

                m_tty.c_cflag &= ~PARENB; /* Disables the Parity   Enable bit(PARENB),So No Parity   */
                m_tty.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
                m_tty.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
                m_tty.c_cflag |= CS8;     /* Set the data bits = 8                                 */

                m_tty.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
                m_tty.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

                m_tty.c_iflag &= ~(IXON | IXOFF | IXANY);         /* Disable XON/XOFF flow control both i/p and o/p */
                // m_tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

                m_tty.c_oflag &= ~OPOST; /*No Output Processing*/

                // // 
                // m_tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                //                 | INLCR | IGNCR | ICRNL | IXON);
                // m_tty.c_oflag &= ~OPOST;
                // m_tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
                // m_tty.c_cflag &= ~(CSIZE | PARENB);
                // m_tty.c_cflag |= CS8;

                /* Setting Time outs */
                m_tty.c_cc[VMIN] = 10; /* Read at least 10 characters */
                m_tty.c_cc[VTIME] = 0; /* Wait indefinetly   */

                if ((tcsetattr(m_serial_port, TCSANOW, &m_tty)) != 0) /* Set the attributes to the termios structure*/
                    // TODO: Throw error
                    throw std::invalid_argument("");
            }

            /**
             * @brief Connect to Serial port
             */
            void connect()
            {
                /* !!blocks the read  */
                /* O_RDWR Read/Write access to serial port */
                /* O_NOCTTY - No terminal will control the process   */
                /* O_NDELAY -Non Blocking Mode,Does not care about-  */
                /* -the status of DCD line,Open() returns immediatly */
                m_serial_port = open(m_com_dev.c_str(), O_RDWR | O_NOCTTY | O_SYNC); 
                if (m_serial_port < 0)
                    throw std::invalid_argument(m_com_dev + " - No device found!");
            }

        public:
            /**
             * @brief Construct a new USART object
             * 
             * @param com_dev COM Device path (e.g. /dev/ttyACM0)
             */
            USART(
                string& com_dev
            ) : COMS::Abstracts::COM_Abstract(
                    com_dev
                )
            { }

            /**
             * @brief Destroy the USART object
             */
            virtual ~USART() = default;

            /**
             * @brief Test connection with device
             * 
             * @return true 
             * @return false 
             */
            bool test_connection()
            {
                m_serial_port = open(m_com_dev.c_str(), O_RDWR | O_NOCTTY | O_SYNC);

                if (m_serial_port < 0)
                    return false;

                close(m_serial_port);

                return true;
            }
    };
}








#endif
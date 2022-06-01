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
             * @param com_dev COM Device path (e.g. /dev/tty0)
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
}








#endif
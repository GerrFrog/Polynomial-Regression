/**
 * @file abstracts.hpp
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains abstracts classes for COM Readers
 * @version 1.0
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022
 */

#pragma once

#ifndef KERNEL_COMS_ABSTRACTS_HEADER
#define KERNEL_COMS_ABSTRACTS_HEADER

#include "../../common.hpp"

#include <sys/ioctl.h>
#include <termios.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief All abstracts classes for COM readers
 */
namespace COMS::Abstracts
{
    /**
     * @brief Abstract class for COM reader
     */
    class COM_Abstract
    {
        private:

        protected:
            /**
             * @brief COM Device string (e.g. /dev/ttyACM1)
             */
            string m_com_dev;

            /**
             * @brief Termios struct in order to configure the serial port.
             */
            struct termios m_tty; // struct termios2 tty

            /**
             * @brief Serial port descriptor;
             */
            int m_serial_port = 0;

            /**
             * @brief End char of data. Read/Write while symbol is not met
             */
            char m_end_char = '\0';

            /**
             * @brief Reading buffer.
             */
            char m_buf = '\0';

            /**
             * @brief Configure the Serial Port
             */
            virtual void configure_sp() = 0;

            /**
             * @brief Connect to Serial Port
             */
            virtual void connect() = 0;

        public:
            COM_Abstract(
                string& com_dev
            ) : m_com_dev(com_dev)
            {
                memset(&m_tty, 0, sizeof(m_tty));
            }

            /**
             * @brief Destroy the com device object
             */
            virtual ~COM_Abstract()
            { 
                close(m_serial_port);
            }

            /**
             * @brief Set the end char for char array.
             * 
             * @param end End symbol for received data. Read/Write while symbol is not met
             */
            void set_end(char end) { m_end_char = end; }

            /**
             * @brief Initialize device and Serial port
             */
            void initialize()
            {
                connect();
                configure_sp();
            }

            /**
             * @brief Read data from serial port
             * 
             * @param read_buf Buffer for reading
             * @return int Number of bytes read
             */
            char* read_data(int size) const
            {
                char read_buffer[size];

                if (read(m_serial_port, &read_buffer, sizeof(read_buffer)) < 0)
                    // TODO: Throw error
                    throw std::invalid_argument("Error reading" + (string)strerror(errno));

                char *p = read_buffer;

                return p;
            }

            /**
             * @brief Write data to Serial port
             * 
             * @param data Data char array
             * @param size Size of data
             * @return int Total send bytes
             */
            int write_data(char data[], int size) const
            {
                char arr[size];
                for (int i = 0; i < size; i++)
                    arr[i] = data[i];
                return write(m_serial_port, arr, size - 1);
            }

            /**
             * @brief Test connection with device
             * 
             * @return true 
             * @return false 
             */
            virtual bool test_connection() = 0;
    };
}









#endif
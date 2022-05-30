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
            std::string com_dev;

            /**
             * @brief Termios struct in order to configure the serial port.
             */
            struct termios tty; // struct termios2 tty

            /**
             * @brief Serial port descriptor;
             */
            int serial_port = 0;

            /**
             * @brief End char of data. Read/Write while symbol is not met
             */
            char end_char = '\0';

            /**
             * @brief Reading buffer.
             */
            char buf = '\0';

            /**
             * @brief Configure the Serial Port
             */
            virtual void configure_sp() = 0;

            /**
             * @brief Connect to Serial Port
             */
            virtual void connect() = 0;

        public:
            /**
             * @brief Construct a new com device object
             */
            COM_Abstract() = default;

            /**
             * @brief Destroy the com device object
             */
            virtual ~COM_Abstract()
            { 
                close(this->serial_port);
            }

            /**
             * @brief Set the end char for char array.
             * 
             * @param end End symbol for received data. Read/Write while symbol is not met
             */
            void set_end(char end) { this->end_char = end; }

            /**
             * @brief Initialize device and Serial port
             */
            void initialize()
            {
                this->connect();
                std::cout << "[+] Serial port successfully connected" << std::endl;
                this->configure_sp();
                std::cout << "[+] Serial port successfully configured" << std::endl;
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

                if (read(this->serial_port, &read_buffer, sizeof(read_buffer)) < 0)
                {
                    std::cout << "Error reading: " << strerror(errno) << std::endl;
                    throw std::invalid_argument("");
                }

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
                return write(this->serial_port, arr, size - 1);
            }

            /**
             * @brief Test connection
             */
            virtual void test_connection() = 0;
    };
}









#endif
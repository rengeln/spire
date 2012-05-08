////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2012 Robert Engeln (engeln@gmail.com) All rights reserved.
//  See accompanying LICENSE file for full license information.
////////////////////////////////////////////////////////////////////////////////
///
/// @file spire/common/exception.hpp
/// Defines the Exception and Error classes.
/// @addtogroup common
/// @{

#ifndef SPIRE_COMMON_EXCEPTION_HPP
#define SPIRE_COMMON_EXCEPTION_HPP

namespace spire
{
    namespace common
    {
        ///
        /// Base class for all custom exceptions.
        ///
        class Exception : public std::exception,
                          public boost::exception
        {
        public:
            ///
            /// Copy constructor.
            ///
            Exception(const Exception& rhs);

            ///
            /// Move constructor.
            ///
            Exception(Exception&& rhs);

            ///
            /// Destructor.
            ///
            ~Exception();

            ///
            /// Appends a string to the error message.
            /// @{
            Exception& operator<<(const std::string& msg);
            Exception& operator<<(const boost::format& msg);
            //! @}

            ///
            /// Returns the error message.
            ///
            const char* what() const throw();

        protected:
            ///
            /// Constructor.
            ///
            /// @param msg Error message.
            ///
            Exception(std::string msg);

        private:
            std::string m_message;
        };

        ///
        /// Template for creating specialized exception classes.
        ///
        /// @tparam Tag Tag type; unused.
        /// @tparam Base Type to inherit from; the default is Exception.
        ///
        template <typename Tag, typename Base = Exception>
        class Error : public Base
        {
        public:
            ///
            /// Constructor.
            ///
            /// @param msg Error message.
            /// @{
            Error(std::string msg)
            : Base(std::move(msg))
            {
            }

            Error(const boost::format& msg)
            : Base(msg.str())
            {
            }
            //! @}
        };

        ///
        /// @class spire::common::SystemError
        /// @brief Base type for system errors.
        ///
        /// System errors are generated by unexpected failures within 3rd
        /// party APIs.
        ///
        typedef Error<struct _SystemError> SystemError;

        ///
        /// @class spire::common::DataError
        /// @brief Base type for data errors.
        ///
        /// Data errors are generated by failures during parsing, loading,
        /// or otherwise handling some data source.
        ///
        typedef Error<struct _DataError> DataError;

        ///
        /// @class spire::common::RuntimeError
        /// @brief Base type for runtime errors.
        ///
        /// Runtime errors are generated by errors in program logic which do
        /// not fall under SystemError or DataError.
        ///
        typedef Error<struct _RuntimeError> RuntimeError;

        ///
        /// Error checking macro.
        ///
        /// @param except Exception to throw on failure.
        /// @param expr Expression to evaluate.
        ///
#       define CHECK(except, expr) if (!(expr)) throw except("Assert failed: " #expr);

        ///
        /// @class spire::common::Win32Error
        /// @brief Specialized exception for Win32 errors.
        ///
        typedef Error<struct _Win32Error, SystemError> Win32Error;

        ///
        /// @class spire::common::Win32ErrorInfo
        /// @brief Enable a Win32 error code to be attached to an exception object.
        ///
        typedef boost::error_info<struct _Win32ErrorInfo, DWORD> Win32ErrorInfo;

        ///
        /// Error checking macro for Win32 functions.
        ///
        /// @param expr Expression to evaluate; should be a Win32 function call
        ///             which evaluates to false on failure and stores the
        ///             error code in GetLastError().
        ///
#       define WIN_CHECK(expr) if (!(expr)) throw spire::common::Win32Error("Assert failed: " #expr) << spire::common::Win32ErrorInfo(GetLastError());
    }   //  namespace common
    using common::Exception;
    using common::Error;
    using common::SystemError;
    using common::RuntimeError;
    using common::DataError;
    using common::Win32Error;
}   //  namespace spire

#endif  //  SPIRE_COMMON_EXCEPTION_HPP
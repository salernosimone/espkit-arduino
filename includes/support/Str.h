#pragma once

#include <random>
#include <string>
#include <cctype>

namespace esptoolkit {
    /**
     * String-related utilities
     */
    class Str {
    public:
        /**
         * Factory
         * @tparam T
         * @param value
         * @return
         */
        template<typename T>
        static Str from(T value) {
            return Str(String(value));
        }

        /**
         * Generate random string
         * @param length
         * @return
         */
        static String random(const uint8_t length = 16) {
            static const char alphanum[] =
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789";
            String s;

            s.reserve(length);
            randomSeed(millis());

            // first characther is always upper letter
            s += alphanum[::random(0, 26)];

            for (uint8_t i = 1; i < length; i++)
                s += alphanum[::random(0, 62)];

            return s;
        }

        /**
         * Constructor
         * @param source
         */
        Str(const String &source) : s(source) {

        }

        /**
         * Test equality
         * @param other
         * @return
         */
        bool operator==(const String &other) const {
            return s == other;
        }

        /**
         * Get underlying string
         * @return
         */
        String &toString() {
            return s;
        }

        /**
         * To lower case
         */
        inline void lower() {
            s.toLowerCase();
        }

        /**
         * To upper case
         */
        inline void upper() {
            s.toUpperCase();
        }

        /**
         * Test if string contains other string
         * @param other
         * @return
         */
        inline bool contains(const String &other) const {
            return s.indexOf(other) > -1;
        }

        /**
         * Join parts (stop condition)
         * @tparam T
         * @tparam Args
         * @param glue
         * @param arg
         * @param args
         * @return
         */
        template<typename T, typename... Args>
        Str &join(String glue, T arg) {
            return concat(glue, arg);
        }

        /**
         * Join parts
         * @tparam T
         * @tparam Args
         * @param glue
         * @param arg
         * @param args
         * @return
         */
        template<typename T, typename... Args>
        Str &join(String glue, T arg, Args... args) {
            concat(glue, arg);

            return join(glue, args...);
        }

        /**
         * Concat stop condition (generic)
         * @tparam T
         * @param value
         * @return
         */
        template<typename T>
        Str &concat(T value) {
            s += value;

            return *this;
        };

        /**
         * Concat stop condition (generic)
         * @tparam T
         * @param condition
         * @param value
         * @return
         */
        template<typename T>
        Str &concatIf(bool condition, T value) {
            if (condition)
                s += value;

            return *this;
        };

        /**
         * Concat segments into a string
         * @tparam T
         * @tparam Args
         * @param value
         * @param args
         * @return
         */
        template<typename T, typename... Args>
        Str &concat(T value, Args... args) {
            concat(value);

            return concat(args...);
        }

        /**
         * Parse duration string. Return in millis
         * @param duration
         * @return
         */
        float duration() {
            const float value = s.toFloat();

            s.toLowerCase();

            if (contains("ms") || contains("millis"))
                return value;

            if (contains("s"))
                return value * 1000;

            if (contains("m"))
                return value * 1000 * 60;

            if (contains("h"))
                return value * 1000 * 3600;

            return value;
        }

        /**
         * Parse string as size (b, kb, mb...)
         * @return
         */
        uint32_t size() {
            const uint32_t value = s.toInt();

            s.toLowerCase();

            if (contains("g"))
                return value * 1073741824ULL;

            if (contains("m"))
                return value * 1048576ULL;

            if (contains("k"))
                return value * 1024;

            return value;
        }

        /**
         * Run function on each exploded substring
         * @tparam Callback
         * @param sep
         * @param callback
         */
        template<typename Callback>
        void explode(String sep, Callback callback) {
            int16_t fromIndex = 0;

            do {
                const int16_t index = s.indexOf(sep, fromIndex);

                if (index < 0) {
                    callback(s.substring(fromIndex));
                    break;
                } else
                    callback(s.substring(fromIndex, index));

                fromIndex = index + 1;
            } while (fromIndex >= 0 && fromIndex < s.length());

        }

        /**
         * Reserve memory
         * @param size
         */
        inline void reserve(size_t size) {
            s.reserve(size);
        }

    protected:
        String s;
    };
}
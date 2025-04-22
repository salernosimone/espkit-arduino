#pragma once

namespace espkit::support {
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
                return value * 1000000000ULL;

            if (contains("m"))
                return value * 1000000;

            if (contains("k"))
                return value * 1000;

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

                if (index < 0)
                    callback(s.substring(fromIndex));
                else
                    callback(s.substring(fromIndex, index));

                fromIndex = index;
            } while (fromIndex >= 0);

        }

    protected:
        String s;
    };
}
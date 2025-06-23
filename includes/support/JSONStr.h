#pragma once

#include "./Str.h"

using esptoolkit::Str;

/**
 * Helper class to build JSON strings
 */
class JSONStr {
public:

    /**
     * Constructor
     */
    JSONStr() : s(""), isFirstProp(true) {

    }

    /**
     *
     * @return
     */
    inline String &toString() {
        return s.toString();
    }

    /**
     *
     * @tparam Callable
     * @param callable
     */
    template<typename Callable>
    void object(Callable callable) {
        openObject();
        callable(*this);
        closeObject();
    }

    /**
     *
     * @param key
     * @param value
     */
    void openObject() {
        isFirstProp = true;
        s.concat("{");
    }

    /**
     *
     * @param key
     * @param value
     */
    void attr(const String &key, int value) {
        String v(value);

        addRawProp(key, v);
    }

    /**
     *
     * @param key
     * @param value
     */
    void attr(const String &key, float value) {
        String v(value, 5);

        addRawProp(key, v);
    }

    /**
     *
     * @param key
     * @param value
     */
    void attr(const String &key, String &value) {
        String v = String('"') + value + '"';
        addRawProp(key, v);
    }

    /**
     *
     * @param key
     * @param value
     */
    void attr(const String &key, const String &value) {
        String v = String('"') + value + '"';
        addRawProp(key, v);
    }

    /**
     *
     * @param key
     * @param value
     */
    void addRawProp(const String &key, String &value) {
        addComma();
        s.concat("\"", key, "\":", value);
    }

    /**
     *
     */
    void closeObject() {
        s.concat("}");
    }

    /**
     * Reserve memory
     * @param size
     */
    inline void reserve(size_t size) {
        s.reserve(size);
    }

protected:
    Str s;
    bool isFirstProp;

    /**
     *
     */
    void addComma() {
        if (!isFirstProp)
            s.concat(",");

        isFirstProp = false;
    }
};

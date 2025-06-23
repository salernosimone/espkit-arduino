#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "../constants.h"

namespace esptoolkit {
    /**
     * Utilities to read/write preferences
     */
    class PreferencesKit {
    public:
        ::Preferences preferences;

        /**
         * Constructor
         * @param ns_
         */
        PreferencesKit(const String &ns_) : ns(ns_) {

        }

        /**
         * Get string value
         * @param key
         * @return
         */
        String getString(const String &key) {
            preferences.begin(ns.c_str(), true);
            const String value = preferences.getString(key.c_str(), "");
            preferences.end();

            return value;
        }

        /**
         * Get numeric value
         * @param key
         * @return
         */
        float getNumber(const String &key, float defaultValue = 0) {
            preferences.begin(ns.c_str(), true);
            const float value = preferences.getFloat(key.c_str(), defaultValue);
            preferences.end();

            return value;
        }

        /**
         * Save string
         * @param key
         * @param value
         */
        void put(const String &key, const String &value) {
            preferences.begin(ns.c_str(), false);
            preferences.putString(key.c_str(), value);
            preferences.end();
        }

        /**
         * Save number
         * @param key
         * @param value
         */
        void put(const String &key, const float value) {
            preferences.begin(ns.c_str(), false);
            preferences.putFloat(key.c_str(), value);
            preferences.end();
        }

    protected:
        const String ns;
    };
}

// global singleton
static esptoolkit::PreferencesKit prefsKit(PREF_NS);
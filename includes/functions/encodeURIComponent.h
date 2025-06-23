#pragma once

/**
 * Mimics encodeURIComponent from Javascript
 * @param str
 * @return
 */
String encodeURIComponent(const String &str) {
    String encoded = "";
    const char *hex = "0123456789ABCDEF";

    for (size_t i = 0; i < str.length(); i++) {
        char c = str.charAt(i);

        // Unreserved characters (do not encode)
        if ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '!' ||
            c == '~' || c == '*' || c == '\'' || c == '(' || c == ')') {
            encoded += c;
        } else {
            // Percent-encode other characters
            encoded += '%';
            encoded += hex[(c >> 4) & 0xF];
            encoded += hex[c & 0xF];
        }
    }

    return encoded;
}
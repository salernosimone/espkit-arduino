#ifndef ELOQUENTESP32_INCLUDE_BROWNOUT_H
#define ELOQUENTESP32_INCLUDE_BROWNOUT_H

#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>


namespace espkit::cam {
    /**
     * Disable brownout detector
     * (often triggers on AiThinker cam)
     */
    class Brownout {
    public:
        /**
         * Disable detector
         */
        void disable() {
            WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
        }

        /**
         * Enable detector
         */
        void enable() {
            WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1);
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_BROWNOUT_H

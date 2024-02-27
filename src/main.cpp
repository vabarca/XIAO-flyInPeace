/**
 *  @brief XIAO-flyInPeace project.
 *  @author Cooked by Vicente A. (TT)
 */

//---[ Includes: ]--------------------------------------------------------------

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "config.h"
#include <barometer/barometer.h>
#include <commons.h>
#include <iomanip>
#include <sound/sound.h>
#include <variometer/variometer.h>
#include <visualizer/visualizer.h>
#include <curve/curve.h>

using namespace vaf::fip;

//---[ Globals: ]---------------------------------------------------------------

static CfipVario *variometer{nullptr};
static CfipSound *sound{nullptr};
static CfipVisualizer *visualizer{nullptr};
static CfipBarometer *barometer{nullptr};
static CfipCurve *curve{nullptr};

//---[ Declarations: ]----------------------------------------------------------

static void initialize_fip_object(CfipObj *obj);

//------------------------------------------------------------------------------

void setup()
{
    SERIAL_BEGIN;

    sound = CFactorySound::create(SOUND_TYPE_AD9833);
    initialize_fip_object(sound);

    visualizer = CFactoryVisualizer::create(VISUALIZER_TYPE_NEOPIXEL_SINGLE, STRIP_PIN);
    initialize_fip_object(visualizer);

    barometer = CFactoryBarometer::create(BARO_TYPE_MS5611);
    initialize_fip_object(barometer);

    variometer = CFactoryVariometer::create(VARIO_TYPE_V1, *barometer);
    initialize_fip_object(variometer);

    curve = CFactoryCurve::create(CURVE_TYPE_V1);
    initialize_fip_object(curve);
}

static void initialize_fip_object(CfipObj *obj)
{
    std::stringstream ss;
    ss << " Initializing " << std::string(obj->objType()) << " ...";
    do
    {
        SERIAL_PRINTLN(ss.str().c_str());
        delay(100);
    } while (ERR_CODE_NONE != obj->setup());
}

void loop()
{
    variometer->update();
    curve->update(variometer->get());
    float vario = curve->get();
    sound->update(vario);
    visualizer->update(vario);

    std::stringstream ss;
    barometer->print(ss);
    variometer->print(ss);
    visualizer->print(ss);
    curve->print(ss);
    SERIAL_PRINTLN(ss.str().c_str());
}

//------------------------------------------------------------------------------

// -- END OF FILE --

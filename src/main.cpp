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
static CfipCurve *sound_curve{nullptr};
static CfipCurve *visualization_curve{nullptr};

//---[ Declarations: ]----------------------------------------------------------

static void initialize_fip_object(CfipObj *obj);

//------------------------------------------------------------------------------

void setup()
{
    SERIAL_BEGIN;

    sound = CFactorySound::create(SOUND_TYPE_AD9833);
    initialize_fip_object(sound);

    visualizer = CFactoryVisualizer::create(VISUALIZER_TYPE_NEOPIXEL_V1, STRIP_PIN);
    initialize_fip_object(visualizer);

    barometer = CFactoryBarometer::create(BARO_TYPE_MS5611);
    initialize_fip_object(barometer);

    variometer = CFactoryVariometer::create(VARIO_TYPE_V1, *barometer);
    initialize_fip_object(variometer);

    sound_curve = CFactoryCurve::create(CURVE_TYPE_V1);
    initialize_fip_object(sound_curve);

    visualization_curve = CFactoryCurve::create(CURVE_TYPE_V1);
    initialize_fip_object(visualization_curve);
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


float cnt = 0.0f;
void loop()
{
    //variometer->update();
    //float vario = variometer->get_norm();

    //sound_curve->update(vario);
    sound_curve->update(cnt);
    //sound->update(sound_curve->get());

    //visualization_curve->update(vario);
    visualization_curve->update(cnt);
    visualizer->update(visualization_curve->get());

    std::stringstream ss;
    barometer->print(ss);
    variometer->print(ss);
    visualizer->print(ss);
    sound_curve->print(ss);
    SERIAL_PRINTLN(ss.str().c_str());

    cnt+=0.1f;
    if(cnt > 1.0f) cnt = 0.0f;
    delay(250);
}

//------------------------------------------------------------------------------

// -- END OF FILE --

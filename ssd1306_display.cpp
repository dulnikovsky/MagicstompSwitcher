
#include "ssd1306_display.h"

SSD1306Display::SSD1306Display(const char *i2c_dev, QObject *parent)
    :QObject(parent)
{
    oled = ssd1306_i2c_open(i2c_dev, 0x3c, 128, 64, NULL);
    if (!oled) {
        return;
    }
    if (ssd1306_i2c_display_initialize(oled) < 0) {
        qDebug("ERROR: Failed to initialize the display. Check if it is connected !");
        ssd1306_i2c_close(oled);
        return;
    }
    fbp = ssd1306_framebuffer_create(oled->width, oled->height, oled->err);

    ssd1306_i2c_display_clear(oled);
}

void SSD1306Display::setCurrentProgram(unsigned char val)
{
    val++;
    programNumStr[0] = '0' + val/10;
    programNumStr[1] = '0' + val%10;

    updateDisplay();
}

void SSD1306Display::onCurrentPatchChanged(unsigned int id, const QString &name, bool isRequesting)
{
    const QByteArray nameByteArr = name.toLocal8Bit();
    currentPatchNamesMap.insert(id, nameByteArr);
    isRequestingState = isRequesting;

    updateDisplay();
}

void SSD1306Display::updateDisplay()
{
    ssd1306_framebuffer_box_t bbox;

    if(param_edit_fbp != nullptr) {

        ssd1306_i2c_display_update(oled, param_edit_fbp);
        return;
    }
    ssd1306_framebuffer_clear(fbp);
    ssd1306_framebuffer_draw_text(fbp, programNumStr, 2, 0, 24, SSD1306_FONT_VERA_BOLD, 6, &bbox);

    uint8_t x = 0;
    uint8_t y = bbox.bottom+16;
    QMap<int, QByteArray>::const_iterator it;
    for (it = currentPatchNamesMap.cbegin(); it != currentPatchNamesMap.cend(); ++it) {

        ssd1306_framebuffer_draw_text(fbp, it.value().constData(), it.value().size(), x, y, SSD1306_FONT_DEFAULT, 4, &bbox);
        y += 16;
    }

    ssd1306_i2c_display_update(oled, fbp);

    if(lastRequestingState != isRequestingState) {
        if(isRequestingState)
            ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_INVERTED, 0, 0);
        else
            ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_NORMAL, 0, 0);
        lastRequestingState = isRequestingState;
    }
}

void SSD1306Display::onMsDisconnected(unsigned int id)
{
    currentPatchNamesMap.remove(id);
    updateDisplay();
}

void SSD1306Display::setInverted(bool val)
{
    if(val)
        ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_INVERTED, 0, 0);
    else
        ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_NORMAL, 0, 0);
}

void SSD1306Display::setParamEditMode(bool val)
{
    if(val) {
        param_edit_fbp = ssd1306_framebuffer_create(oled->width, oled->height, oled->err);
    } else {
        if(param_edit_fbp) {
            ssd1306_framebuffer_destroy(param_edit_fbp);
            param_edit_fbp = nullptr;
        }
    }
    updateDisplay();
}

bool SSD1306Display::drawParamVal(const QByteArray &paramter, const QByteArray &val)
{
    if(param_edit_fbp == nullptr)
        return false;

    ssd1306_framebuffer_box_t bbox;

    ssd1306_framebuffer_clear(param_edit_fbp);
    ssd1306_framebuffer_draw_text(param_edit_fbp, paramter.constData(), paramter.size(), 0, 24, SSD1306_FONT_DEFAULT, 4, &bbox);
    ssd1306_framebuffer_draw_text(param_edit_fbp, val.constData(), val.size(), 32, 48, SSD1306_FONT_DEFAULT, 4, &bbox);
    updateDisplay();

    return true;
}

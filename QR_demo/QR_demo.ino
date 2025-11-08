/*  esp32cam_qr_demo_fast.ino  |  18 Jul 2025
    Faster local QR detection on ESP32‑CAM + quirc
*/

#include <Arduino.h>
#include "esp_camera.h"
#include "quirc.h"
#include <math.h>

// ────────── AI‑Thinker pin map ──────────
#define PWDN_GPIO_NUM   32
#define RESET_GPIO_NUM  -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM   26
#define SIOC_GPIO_NUM   27
#define Y9_GPIO_NUM     35
#define Y8_GPIO_NUM     34
#define Y7_GPIO_NUM     39
#define Y6_GPIO_NUM     36
#define Y5_GPIO_NUM     21
#define Y4_GPIO_NUM     19
#define Y3_GPIO_NUM     18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM  25
#define HREF_GPIO_NUM   23
#define PCLK_GPIO_NUM   22

// ────────── Camera config ──────────
static camera_config_t cam_cfg = {
  .pin_pwdn     = PWDN_GPIO_NUM,
  .pin_reset    = RESET_GPIO_NUM,
  .pin_xclk     = XCLK_GPIO_NUM,
  .pin_sscb_sda = SIOD_GPIO_NUM,
  .pin_sscb_scl = SIOC_GPIO_NUM,
  .pin_d7       = Y9_GPIO_NUM,
  .pin_d6       = Y8_GPIO_NUM,
  .pin_d5       = Y7_GPIO_NUM,
  .pin_d4       = Y6_GPIO_NUM,
  .pin_d3       = Y5_GPIO_NUM,
  .pin_d2       = Y4_GPIO_NUM,
  .pin_d1       = Y3_GPIO_NUM,
  .pin_d0       = Y2_GPIO_NUM,
  .pin_vsync    = VSYNC_GPIO_NUM,
  .pin_href     = HREF_GPIO_NUM,
  .pin_pclk     = PCLK_GPIO_NUM,
  .xclk_freq_hz = 24000000,
  .ledc_timer   = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  .pixel_format = PIXFORMAT_GRAYSCALE,
  .frame_size   = FRAMESIZE_QVGA,   // FRAMESIZE_QQVGA = 160×120, FRAMESIZE_QVGA = 320 x 240, FRAMESIZE_VGA = 640 x 480
  .jpeg_quality = 12,
  .fb_count     = 2,                 // will be overwritten below if no PSRAM
  .grab_mode    = CAMERA_GRAB_LATEST
};

// ────────── Globals ──────────
static struct quirc *qr = nullptr;
static int img_w = 0, img_h = 0;

static inline int dist(int x0,int y0,int x1,int y1){
  int dx=x1-x0, dy=y1-y0;
  return (int)roundf(sqrtf((float)(dx*dx+dy*dy)));
}

// ────────── Init helpers ──────────
static void init_camera()
{
  if (!psramFound()) cam_cfg.fb_count = 1;       // single buffer if no PSRAM

  esp_err_t err = esp_camera_init(&cam_cfg);
  if (err != ESP_OK) {
    Serial.printf("[ERR] Cam init 0x%X\n", err);
    while (true) delay(1);
  }
  camera_fb_t *fb = esp_camera_fb_get();
  img_w = fb->width; img_h = fb->height;
  esp_camera_fb_return(fb);
  Serial.printf("[OK] Cam %dx%d GRAY, fb=%d\n",
                img_w, img_h, cam_cfg.fb_count);
}

static void init_quirc()
{
  qr = quirc_new();
  if (!qr || quirc_resize(qr, img_w, img_h) < 0) {
    Serial.println("[ERR] quirc alloc/resize");
    while (true) delay(1);
  }
  Serial.println("[OK] quirc ready");
}

// ────────── QR task ──────────
void qrTask(void*)
{
  static struct quirc_code code;
  static struct quirc_data data;

  for (;;)
  {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) { vTaskDelay(1); continue; }

    // copy 19 kB → about 0.2 ms on 240 MHz core
    uint8_t *dst = quirc_begin(qr, nullptr,nullptr);
    memcpy(dst, fb->buf, img_w*img_h);
    quirc_end(qr);
    esp_camera_fb_return(fb);

    int n = quirc_count(qr);
    for (int i=0;i<n;++i){
      quirc_extract(qr,i,&code);
      if (quirc_decode(&code,&data)==QUIRC_SUCCESS){
        int cx=0,cy=0;
        for(int k=0;k<4;++k){ cx+=code.corners[k].x; cy+=code.corners[k].y; }
        cx>>=2; cy>>=2;
        int w=dist(code.corners[0].x,code.corners[0].y,
                   code.corners[1].x,code.corners[1].y);
        int h=dist(code.corners[1].x,code.corners[1].y,
                   code.corners[2].x,code.corners[2].y);
        
        // Output structured format: QR:<id>,<cx>,<cy>,<width>,<height>\n
        // Extract QR code ID from payload (first part before comma, or entire payload)
        Serial.print("QR:");
        // Print payload as ID (will be parsed on Arduino side)
        for(int j=0; j<data.payload_len; j++) {
          Serial.print((char)data.payload[j]);
        }
        Serial.printf(",%d,%d,%d,%d\n", cx, cy, w, h);
      }
      taskYIELD();                        // feed watchdog
    }
    vTaskDelay(1);                        // only 1 ms pause now
  }
}

// ────────── Arduino setup/loop ──────────
void setup()
{
  Serial.begin(115200); delay(300);
  Serial.println("\n--- Fast ESP32‑CAM QR Demo ---");
  init_camera();
  init_quirc();

  const uint32_t STACK_WORDS = 16*1024;   // 64 kB
  xTaskCreatePinnedToCore(qrTask,"qrTask",
                          STACK_WORDS,nullptr,4,nullptr,1);
}
void loop(){ vTaskDelay(1000); }

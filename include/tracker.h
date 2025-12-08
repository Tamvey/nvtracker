/*
 * Copyright (c) 2018-2020, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef GST_APP_H
#define GST_APP_H

#include <gst/gst.h>
#include <glib.h>

// Configuration file names
#define PGIE_CONFIG_FILE  "configs/dstest2_pgie_config.txt"
#define SGIE1_CONFIG_FILE "configs/dstest2_sgie1_config.txt"
#define SGIE2_CONFIG_FILE "configs/dstest2_sgie2_config.txt"
#define SGIE3_CONFIG_FILE "configs/dstest2_sgie3_config.txt"
#define MAX_DISPLAY_LEN 64

#define TRACKER_CONFIG_FILE "configs/dstest2_tracker_config.txt"
#define MAX_TRACKING_ID_LEN 16

#define PGIE_CLASS_ID_VEHICLE 0
#define PGIE_CLASS_ID_PERSON 2

#define MUXER_OUTPUT_WIDTH 1920
#define MUXER_OUTPUT_HEIGHT 1080
#define MUXER_BATCH_TIMEOUT_USEC 40000

// Global variables
extern gint frame_number;

// String arrays for class labels
extern gchar sgie1_classes_str[12][32];
extern gchar sgie2_classes_str[20][32];
extern gchar sgie3_classes_str[6][32];
extern gchar pgie_classes_str[4][32];

// Unique IDs for GIE elements
extern guint sgie1_unique_id;
extern guint sgie2_unique_id;
extern guint sgie3_unique_id;

// Function declarations
static GstPadProbeReturn osd_sink_pad_buffer_probe (GstPad * pad, GstPadProbeInfo * info, gpointer u_data);
static gboolean bus_call (GstBus * bus, GstMessage * msg, gpointer data);
static gboolean set_tracker_properties (GstElement *nvtracker);
static gchar * get_absolute_file_path (gchar *cfg_file_path, gchar *file_path);
int run_tracker (int argc, char *argv[]);

// Tracker config parsing constants
#define CONFIG_GROUP_TRACKER "tracker"
#define CONFIG_GROUP_TRACKER_WIDTH "tracker-width"
#define CONFIG_GROUP_TRACKER_HEIGHT "tracker-height"
#define CONFIG_GROUP_TRACKER_LL_CONFIG_FILE "ll-config-file"
#define CONFIG_GROUP_TRACKER_LL_LIB_FILE "ll-lib-file"
#define CONFIG_GROUP_TRACKER_ENABLE_BATCH_PROCESS "enable-batch-process"
#define CONFIG_GPU_ID "gpu-id"

#define CHECK_ERROR(error) \
    if (error) { \
        g_printerr ("Error while parsing config file: %s\n", error->message); \
        goto done; \
    }

#endif // GST_APP_H
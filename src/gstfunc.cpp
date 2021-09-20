#include <gst/gst.h>
#include <stdio.h>
#include <unistd.h> // sleep
// #include <iostream>
// #include <sstream>
// #include <string.h>

// gcc gst_messages.c -g3 -o gst_messages `pkg-config --cflags --libs gstreamer-0.1`

static GMainLoop *loop;

static gboolean
my_bus_callback(GstBus *bus,
                GstMessage *message,
                gpointer data)
{
    const GstStructure *s;
    double duration;

    //g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));

    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ELEMENT:
        s = gst_message_get_structure(message);
        printf("%s\n", gst_structure_to_string(s));
        gst_structure_get_double(s, "duration", &duration);
        printf("%f\n", duration);
        break;
    case GST_MESSAGE_ERROR:
    {
        GError *err;
        gchar *debug;

        gst_message_parse_error(message, &err, &debug);
        g_print("Error: %s\n", err->message);
        g_error_free(err);
        g_free(debug);

        g_main_loop_quit(loop);
        break;
    }
    case GST_MESSAGE_EOS:
        /* end-of-stream */
        printf("ending\n");
        g_main_loop_quit(loop);
        break;
    default:
        /* unhandled message */
        break;
    }

    /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
    return TRUE;
}

// TODO: Proper teardown
gboolean end_my_pipeline_somehow(gpointer data)
{
    //end the pipeline
    return TRUE;
}

gint take_picture(char *device, char *c)
{
    GstBus *bus;
    guint bus_watch_id;

    GstElement *pipeline = gst_pipeline_new("my_pipeline");
    GstElement *v4l2src = gst_element_factory_make("v4l2src", "src");
    GstElement *jpegenc = gst_element_factory_make("jpegenc", "encoder");
    GstElement *multifilesink = gst_element_factory_make("multifilesink", "multifilesink");
    GstElement *capsfilter = gst_element_factory_make("capsfilter", "caps_filter");

    g_object_set(v4l2src, "device", device, NULL);
    g_object_set(v4l2src, "num_buffers", 1, NULL);
    g_object_set(multifilesink, "location", c, NULL);
    g_object_set(multifilesink, "post-messages", TRUE, NULL);

    // Create caps
    GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                        "format", G_TYPE_STRING, "YUY2", //<<< IF THIS IS SET TO ARGB (THE FORMAT I WANT IT FAILS ON LINKING)
                                        "framerate", GST_TYPE_FRACTION, 30, 1,
                                        "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                        "width", G_TYPE_INT, 640,
                                        "height", G_TYPE_INT, 480,
                                        NULL);

    g_object_set(capsfilter, "caps", caps, NULL);

    gst_bin_add(GST_BIN(pipeline), v4l2src);
    gst_bin_add(GST_BIN(pipeline), capsfilter);
    gst_bin_add(GST_BIN(pipeline), jpegenc);
    gst_bin_add(GST_BIN(pipeline), multifilesink);

    gboolean success;
    success = gst_element_link(v4l2src, capsfilter);
    if (!success)
    {
        g_printerr("Elements could not be linked 1.\n");
    }
    success = gst_element_link(capsfilter, jpegenc);
    if (!success)
    {
        g_printerr("Elements could not be linked 2.\n");
    }
    success = gst_element_link(jpegenc, multifilesink);
    if (!success)
    {
        g_printerr("Elements could not be linked 3.\n");
    }

    /* adds a watch for new message on our pipeline's message bus to
   * the default GLib main context, which is the main context that our
   * GLib main loop is attached to below
   */
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, my_bus_callback, NULL);
    //g_signal_connect (bus, "message::element", G_CALLBACK (my_bus_callback), NULL);
    gst_object_unref(bus);

    /* create a mainloop that runs/iterates the default GLib main context
   * (context NULL), in other words: makes the context check if anything
   * it watches for has happened. When a message has been posted on the
   * bus, the default main context will automatically call our
   * my_bus_callback() function to notify us of that message.
   * The main loop will be run until someone calls g_main_loop_quit()
   */

    loop = g_main_loop_new(NULL, FALSE);
    g_timeout_add_seconds(35, end_my_pipeline_somehow, pipeline);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("running\n");

    g_main_loop_run(loop);

    // sleep(20);

    /* clean up */
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);

    return 0;
}

int32_t main(int32_t argc, char **argv)
{
    /* init */
    gst_init(&argc, &argv);
    for (int i = 0; i < 10; i++)
    {
        time_t t = time(0); // get time now
        struct tm *now = localtime(&t);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d-%T", now);
      
        char filename[100] = {};

        char *c = (char *)".jpg";
        char *n = (char *)"i"; // aqui deberia ir una conversion de la variable i del lazo for
  
        strcat(filename,buffer);
        strcat(filename,n);
        strcat(filename,c);

        take_picture(argv[1], filename);

    }

    return 0;
}

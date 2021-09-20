video/x-raw, format=(string)I420, width=(int)640, height=(int)480, framerate=(fraction)30/1

video/x-raw, format=(string)I420, width=(int)640, height=(int)480, interlace-mode=(string)progressive, multiview-mode=(string)mono, multiview-flags=(GstVideoMultiviewFlagsSet)0:ffffffff:/right-view-first/left-flipped/left-flopped/right-flipped/right-flopped/half-aspect/mixed-mono, pixel-aspect-ratio=(fraction)1/1, chroma-site=(string)jpeg, colorimetry=(string)1:4:0:0, framerate=(fraction)30/1


gst-launch-1.0 v4l2src device=/dev/video2 ! video/x-raw, format=I420, width=640, height=480, framerate=30/1 ! decodebin ! ximagesink


gst-launch-1.0 v4l2src device=/dev/video2 ! image/jpeg,width=640,height=480 ! decodebin ! xvimagesink


gst-launch-1.0 v4l2src device=/dev/video0 ! width=640,height=480 ! jpegenc ! avimux ! filesink location=video.avi


# Funciona para grabar video de la camara
gst-launch-1.0 v4l2src device=/dev/video2 ! image/jpeg, width=640,height=480 ! decodebin ! avimux ! filesink location=video2.avi

# Funciona para ver video de la camara
gst-launch-1.0 v4l2src device=/dev/video2 ! image/jpeg,width=640,height=480 ! decodebin ! xvimagesink

gst-launch-1.0 v4l2src device=/dev/video2 ! image/jpeg,width=640,height=480,framerate=15/2 ! jpegdec ! xvimagesink

gst-launch-1.0 -v v4l2src device=/dev/video2 ! clockoverlay ! autovideosink

# Funciona para ver el video y grabarlo
GST_DEBUG=3 gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg, width=640,height=480 ! decodebin ! tee name=t ! queue ! avimux ! filesink location=video2.avi t. ! queue ! xvimagesink

# Funciona para 2 cosas

gst-launch-1.0 filesrc location=song.ogg ! decodebin ! tee name=t ! queue ! audioconvert ! audioresample ! autoaudiosink t. ! queue ! audioconvert ! goom ! videoconvert ! autovideosink

# Funciona para tomar foto
gst-launch-1.0 v4l2src device=/dev/video2 num-buffers=1 ! jpegenc ! filesink location=test.jpg

# Funciona para tomar raw
gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=1 ! filesink location=test.raw

* El -v permite ver que configuracion funciona

gst-launch-1.0 v4l2src device=/dev/video0 num-buffers=1 ! video/x-raw, format=YUY2, width=1280, height=960, pixel-aspect-ratio=1/1, framerate=15/2, colorimetry=2:4:5:1, interlace-mode=progressive ! filesink location=test.yuy2

## multiples fotos raw
gst-launch-1.0 v4l2src device=/dev/video0 num-buffers=300 ! video/x-raw, format=YUY2, width=1280, height=960, pixel-aspect-ratio=1/1, framerate=15/2, colorimetry=2:4:5:1, interlace-mode=progressive ! multifilesink location=test-%02d.yuy2

## multiples fotos jpeg
gst-launch-1.0 v4l2src device=/dev/video2 num-buffers=10 ! video/x-raw, format=YUY2, width=640, height=480, pixel-aspect-ratio=1/1, framerate=30/1, colorimetry=2:4:5:1, interlace-mode=progressive ! jpegenc ! multifilesink post-messages=true location=test-%02d.jpg

# VER VIDEO DE LA CAMARA, anadir marca de tiemo
gst-launch-1.0 -v v4l2src device=/dev/video0 ! video/x-raw, format=YUY2, width=640, height=480, pixel-aspect-ratio=1/1, framerate=30/1, colorimetry=2:4:5:1, interlace-mode=progressive ! clockoverlay time-format="%D %H:%M:%S.%s" font-desc=\"Sans, 20\" ! videoconvert ! autovideosink




gst-launch-1.0 v4l2src device=/dev/video2 ! xvimagesink

gst-launch-1.0 v4l2src ! video/x-raw-yuv,width=640,height=480 ! ffmpegcolorspace ! jpegenc ! queue ! avimux name=mux ! filesink location=video.avi pulsesrc ! audioconvert ! audio/x-raw-int,rate=44100,channels=2 ! mux.


gst-launch-1.0 v4l2src device=/dev/video2 ! video/x-raw-yuv,width=640,height=480 ! ffmpegcolorspace ! jpegenc ! queue ! avimux name=mux ! filesink location=video.avi pulsesrc ! audioconvert ! audio/x-raw-int,rate=44100,channels=2 ! mux.


Crear multiples archivos
https://stackoverflow.com/questions/43944397/add-timestamps-to-h264-es-video-stream-gstreamer#48012294



# Docker

docker run -ti --privileged -v $PWD/output:/opt/sources/output gstreamerapp:latest

### Time con docker
real    0m2,359s
user    0m0,030s
sys     0m0,034s

### Time sin docker
real    0m1,401s
user    0m0,054s
sys     0m0,009s


# Ver info de la camara
v4l2-ctl --all -d /dev/video0

# Listar dispositivos
v4l2-ctl --list-devices

# camera capabilities 


v4l2-ctl -d /dev/video0 --list-formats-ext



~$ ffmpeg -hide_banner -f v4l2 -list_formats all -i /dev/video0
[video4linux2,v4l2 @ 0x5652617176c0] Compressed:       mjpeg :          Motion-JPEG : 160x120 176x144 320x240 352x288 640x480 960x720
[video4linux2,v4l2 @ 0x5652617176c0] Raw       :     yuyv422 :           YUYV 4:2:2 : 160x120 176x144 320x240 352x288 640x480 960x720 1280x960
/dev/video0: Immediate exit requested



# Integracion

El arduino eniva un "OK" via serial, que puede servir para la interaccion
readOK

* Devolver un ok cuando tome la foto
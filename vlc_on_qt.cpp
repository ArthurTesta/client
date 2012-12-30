
/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */
#include "vlc_on_qt.h"



#include <QX11EmbedContainer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFrame>
#include <QMessageBox>

Player::Player(QString media)
    : QWidget()
{
    //preparation of the vlc command
    const char * const vlc_args[] = {
        "-I", "dummy", /* Don't use any interface */
        "--extraintf=logger", //log anything
        "--verbose=2", //be much more verbose then normal for debugging purpose
        "--plugin-path=/usr/lib/vlc/plugins" };

#ifdef Q_WS_X11
    _videoWidget=new QX11EmbedContainer(this);
#else
    _videoWidget=new QFrame(this);
#endif

    // on set tout ce qui est relatif aux boutons

    _pauseButton = new QPushButton("Pause");
    _playButton = new QPushButton("Play");
    _volumeSlider=new QSlider(Qt::Horizontal,this);
    _volumeSlider->setMaximum(100); //the volume is between 0 and 100
    _volumeSlider->setToolTip("Audio slider");

    /* Note: if you use streaming, there is no ability to use the position slider
      En rtsp ça fonctionne plus ou moins bien
    */

    _positionSlider=new QSlider(Qt::Horizontal,this);
    _positionSlider->setMaximum(POSITION_RESOLUTION);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_videoWidget);
    layout->addWidget(_positionSlider);
    layout->addWidget(_volumeSlider);
    layout->addWidget(_pauseButton);
    layout->addWidget(_playButton);
    setLayout(layout);

    _isPlaying=false;
    poller=new QTimer(this);

    //create a new libvlc instance
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);  //tricky calculation of the char space used

    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);

    // On connecte tous les boutons avec leurs signaux
    connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));
    connect(_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));
    connect(_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
    connect(_pauseButton, SIGNAL(clicked()),this,SLOT(pause()));
    connect(_playButton, SIGNAL(clicked()),this,SLOT(play()));

    poller->start(100); //start timer to trigger every 100 ms the updateInterface slot
    resize(640,480);
    //p.playFile("file:///home/web/sample.mkv"); // Replace with what you want to play
     playFile(media); // Replace with what you want to play
}

//desctructor
Player::~Player()
{
    /* Stop playing */
    libvlc_media_player_stop (_mp);

    /* Free the media_player */
    libvlc_media_player_release (_mp);

    libvlc_release (_vlcinstance);
}

void Player::playFile(QString file)
{
    //the file has to be in one of the following formats /perhaps a little bit outdated)
    /*
    [file://]filename              Plain media file
    http://ip:port/file            HTTP URL
    ftp://ip:port/file             FTP URL
    mms://ip:port/file             MMS URL
    screen://                      Screen capture
    [dvd://][device][@raw_device]  DVD device
    [vcd://][device]               VCD device
    [cdda://][device]              Audio CD device
    udp:[[<source address>]@[<bind address>][:<bind port>]]
    */

    /* Create a new LibVLC media descriptor */
    _m = libvlc_media_new_location (_vlcinstance, file.toAscii());

    libvlc_media_player_set_media (_mp, _m);

    /* /!\ Please note /!\

     passing the widget to the lib shows vlc at which position it should show up
     vlc automatically resizes the video to the ´given size of the widget
     and it even resizes it, if the size changes at the playing

    Get our media instance to use our window */
#if defined(Q_OS_WIN)
    libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(_videoWidget->winId()), &_vlcexcep );
    //libvlc_media_player_set_hwnd(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_drawable(_mp, _videoWidget->winId(), &_vlcexcep );
    //libvlc_media_player_set_agl (_mp, _videoWidget->winId(), &_vlcexcep); // for vlc 1.0
#else //Linux
    //[20101201 Ondrej Spilka] obsolete call on libVLC >=1.1.5
    //libvlc_media_player_set_drawable(_mp, _videoWidget->winId(), &_vlcexcep );
    //libvlc_media_player_set_xwindow(_mp, _videoWidget->winId(), &_vlcexcep ); // for vlc 1.0

    /* again note X11 handle on Linux is needed
        winID() returns X11 handle when QX11EmbedContainer us used */

    int windid = _videoWidget->winId();
    libvlc_media_player_set_xwindow (_mp, windid );

#endif

    /* Play */
 libvlc_media_player_play (_mp);
    _isPlaying=true;
}

void Player::changeVolume(int newVolume)
{
    libvlc_audio_set_volume(_mp,newVolume);
}

void Player::changePosition(int newPosition)
{
    //libvlc_exception_clear(&_vlcexcep);
    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;

    float pos=(float)(newPosition)/(float)POSITION_RESOLUTION;
    libvlc_media_player_set_position (_mp, pos);
}

void Player::pause()
{
    if(_isPlaying){
    libvlc_media_player_set_pause(_mp,1);
    _isPlaying=false;
    }

}

void Player::play()
{
    if(!_isPlaying){
        libvlc_media_player_set_pause(_mp,0);
        _isPlaying=true;
    }
}

void Player::updateInterface()
{
    if(!_isPlaying)
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;
    float pos=libvlc_media_player_get_position (_mp);
    int siderPos=(int)(pos*(float)(POSITION_RESOLUTION));
    _positionSlider->setValue(siderPos);
    int volume=libvlc_audio_get_volume (_mp);
    _volumeSlider->setValue(volume);
}

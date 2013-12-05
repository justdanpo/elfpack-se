#ifndef __IMAGES_INCLUDE__
#define __IMAGES_INCLUDE__


#define IMG_FORWARD 0
#define IMG_REWIND 1
#define IMG_LOOP 2
#define IMG_STEREOWIDENING_OFF 3
#define IMG_RANDOM 4
#define IMG_RANDOM_OFF 5
#define IMG_STEREOWIDENING 6
#define IMG_LOOP_OFF 7

extern wchar_t navigimg;
extern wchar_t presets[];
extern wchar_t media_images[];

#define IMG_PREV 0
#define IMG_NEXT 1
#define IMG_PREV2 2
#define IMG_NEXT2 3
#define IMG_PLAY 4
#define IMG_STOP 5
#define IMG_UP 6
#define IMG_DOWN 7
extern wchar_t images[];

#define ICON(a,b) (a!=NOIMAGE ? a : b)

#define ICN_ARTIST 0
#define ICN_ALBUM 1
#define ICN_TITLE 2
extern wchar_t main_icons[];

#endif

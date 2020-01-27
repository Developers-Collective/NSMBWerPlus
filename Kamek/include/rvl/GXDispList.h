#ifndef __GXDISPLIST_H__
#define __GXDISPLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
void GXBeginDisplayList		( void *list, u32 size );
u32  GXEndDisplayList		( void );
void GXCallDisplayList		( const void *list, u32 nbytes );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXDISPLIST_H__

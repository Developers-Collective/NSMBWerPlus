#ifndef __PPCWGPIPE_H__
#define __PPCWGPIPE_H__

#ifdef  __cplusplus
extern  "C" {
#endif

/*---------------------------------------------------------------------------*
    PPC Write Gather Pipe

    Write Gather Pipe is defined as:
        PPCWGPipe wgpipe : <Write Gathered Address>;

    Then, used as:
        wgpipe.u8  = 0xff;
        wgpipe.s16 = -5;
        wgpipe.f32 = 0.10f;
 *---------------------------------------------------------------------------*/
typedef union uPPCWGPipe
{
    u8  _u8;
    u16 _u16;
    u32 _u32;
    u64 _u64;
    s8  _s8;
    s16 _s16;
    s32 _s32;
    s64 _s64;
    f32 _f32;
    f64 _f64;
} PPCWGPipe;

#ifdef  __cplusplus
}
#endif

#endif  //__PPCWGPIPE_H__

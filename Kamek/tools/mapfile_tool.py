# half assed

# d_profile: same place
# d_bases: same place
# d_enemies and part of d_bases: offset by 0x20
# d_en_boss: offset by 0x20

def make_hex_offset(offs):
    return '0x%08X' % offs

def fix_offs_pal_v2(offs):
    if offs >= 0x800CF6E8 and offs <= 0x800CF90F:
        return offs + 8

    if offs >= 0x807685A0 and offs <= 0x807AAA70:
        return offs + 0x40

    if offs >= 0x807AAA74 and offs <= 0x809907FF:
        return offs + 0x10

    if offs >= 0x80990800:
        return offs + 0x20

    return offs

def fix_offs_ntsc_v1(offs):
    # .text section
    if offs >= 0x800B4604 and offs <= 0x800C8E4C:
        return offs - 0x50

    if offs >= 0x800C8E50 and offs <= 0x800E4D70:
        return offs - 0xF0

    if offs >= 0x800E4EC0 and offs <= 0x8010F200:
        return offs - 0x110

    if offs >= 0x8010F430 and offs <= 0x802BB6BC:
        return offs - 0x140

    if offs >= 0x802BB6C0 and offs <= 0x802BB74C:
        return offs - 0x150

    if offs >= 0x802BB860 and offs <= 0x802BBBFC:
        return offs - 0x260

    if offs >= 0x802BBC90 and offs <= 0x802EDCC0:
        return offs - 0x2F0

    # .ctors, .dtors, .rodata, part of .data section
    if offs >= 0x802EDCE0 and offs <= 0x80317734:
        return offs - 0x300

    # .data section
    if offs >= 0x80317750 and offs <= 0x80322FE0:
        return offs - 0x318

    if offs >= 0x80323118 and offs <= 0x8032E77C:
        return offs - 0x348

    if offs >= 0x8032E780 and offs <= 0x8035197C:
        return offs - 0x340

    # .sdata section, part of .sbss
    if offs >= 0x80351980 and offs <= 0x80427E87:
        return offs - 0x300

    # .sbss, .sdata2, .sbss2 sections
    if offs >= 0x80427E88 and offs <= 0x80429563:
        return offs - 0x310

    if offs >= 0x80429564 and offs <= 0x80429D7F:
        return offs - 0x2F8

    if offs >= 0x80429D80 and offs <= 0x807684BF:
        return offs - 0x2E0

    # part of d_basesNP, d_enemiesNP, d_en_bossNP
    if offs >= 0x8098A43C:
        return offs + 0x20

    return offs


def fix_offs_ntsc_v2(offs):
    offs = fix_offs_ntsc_v1(offs)

    if offs >= 0x800CF5F8 and offs <= 0x800CF81F:
        return offs + 8

    if offs >= 0x807685A0 and offs <= 0x807AAA70:
        return offs + 0x40

    if offs >= 0x807AAA74 and offs <= 0x8099081C:
        return offs + 0x10

    if offs >= 0x80990820:
        return offs + 0x20

    return offs


def fix_offs_jpn_v1(offs):
    # .text section
    if offs >= 0x800B4604 and offs <= 0x800B475C:
        return offs - 0x50

    if offs >= 0x800B4760 and offs <= 0x800C8DAC:
        return offs - 0xD0

    if offs >= 0x800C8E50 and offs <= 0x800E4D6C:
        return offs - 0x170

    if offs >= 0x800E4D94 and offs <= 0x800E4EB4:
        return offs - 0x194

    if offs >= 0x800E4EB8 and offs <= 0x8010F1D0:
        return offs - 0x190

    if offs >= 0x8010F430 and offs <= 0x802BB6BC:
        return offs - 0x330

    if offs >= 0x802BB6D0 and offs <= 0x802BB74C:
        return offs - 0x340

    if offs >= 0x802BB860 and offs <= 0x802BBBFC:
        return offs - 0x450

    if offs >= 0x802BBC90 and offs <= 0x802EDCC0:
        return offs - 0x4E0

    # .ctors, .dtors, .rodata, part of .data section
    if offs >= 0x802EDCE0 and offs <= 0x80317734:
        return offs - 0x4E0

    # .data section
    if offs >= 0x80317750 and offs <= 0x80322FDC:
        return offs - 0x4F8

    if offs >= 0x80323118 and offs <= 0x8035197C:
        return offs - 0x5E0

    # .sdata, part of .sbss section
    if offs >= 0x80351980 and offs <= 0x80427E5F:
        return offs - 0x580

    if offs >= 0x80427E88 and offs <= 0x8042954B:
        return offs - 0x5A8

    if offs >= 0x80429570 and offs <= 0x80429D7F:
        return offs - 0x5C8

    # part of .sdata2, .sbss2 section
    if offs >= 0x80429D80 and offs <= 0x807684BF: # end offset is right before d_profileNP header
        return offs - 0x5C0

    # d_profileNP and d_basesNP
    # "no change" gap ends at 8779ABC
    if offs >= 0x80779C70 and offs <= 0x8078891F:
        return offs - 0x130

    if offs >= 0x80788AD0 and offs <= 0x80789EEF:
        return offs - 0x260

    if offs >= 0x80789F00 and offs <= 0x808D3B87:
        return offs - 0x270

    if offs >= 0x808D3BD4 and offs <= 0x808D3C1F:
        return offs - 0x2B4

    if offs >= 0x808D3C20 and offs <= 0x80940C47:
        return offs - 0x2C0

    if offs >= 0x80940F58 and offs <= 0x80943167:
        return offs - 0x4E8

    if offs >= 0x809431F8 and offs <= 0x8094329F:
        return offs - 0x4F8

    if offs >= 0x809432C0 and offs <= 0x80944E77:
        return offs - 0x500

    if offs >= 0x80945144 and offs <= 0x80945153:
        return offs - 0x714

    if offs >= 0x80945158 and offs <= 0x8098A36B:
        return offs - 0x718

    if offs >= 0x8098A478 and offs <= 0x8098FF18:
        return offs - 0x6F8

    # d_enemiesNP
    # this offset starts at the .rel header
    if offs >= 0x80990800 and offs <= 0xDEADBEEF:
        return offs - 0x700

    #if offs >= 0x8010F430 and offs <= 0xDEADBEEF:
    #   return offs - 0x330

    #if offs >= 0x8010F430 and offs <= 0xDEADBEEF:
    #   return offs - 0x330

    return offs


def fix_offs_jpn_v2(offs):
    offs = fix_offs_jpn_v1(offs)

    if offs >= 0x800CF578 and offs <= 0x800CF79F:
        return offs + 8

    if offs >= 0x807685A0 and offs <= 0x807AA7FF:
        return offs + 0x40

    if offs >= 0x807AA800 and offs <= 0x809900FF:
        return offs + 0x10

    if offs >= 0x80990100:
        return offs + 0x20

    return offs


import yaml

original = 'pal'
fix_for = {
    'pal2': fix_offs_pal_v2,
    'ntsc': fix_offs_ntsc_v1,
    'ntsc2': fix_offs_ntsc_v2,
    'jpn': fix_offs_jpn_v1,
    'jpn2': fix_offs_jpn_v2
}

def do_mapfile(src, dest, fix_offset):
    map = [x.rstrip() for x in open(src, 'r')]
    new = []

    for line in map:
        if ' = 0x80' in line:
            pos = line.find('0x80')
            offs = int(line[pos:pos+10], 0)
            offs = fix_offset(offs)
            line = line[:pos] + make_hex_offset(offs) + line[pos+10:]

        new.append(line + '\n')

    open(dest, 'w').writelines(new)


def work_on_hook(hook, id, func):

    try:
        t = hook['type']

        if t == 'patch':
            error = 'missing addr'

            hook['addr_%s' % id] = func(hook['addr_%s' % original])

        elif t == 'branch_insn' or t == 'add_func_pointer':
            error = 'missing src_addr'

            hook['src_addr_%s' % id] = func(hook['src_addr_%s' % original])

            if 'target_func_%s' % original in hook:
                error = 'missing target_func'

                hook['target_func_%s' % id] = func(hook['target_func_%s' % original])

        elif t == 'nop_insn':
            error = 'missing area'

            area = hook['area_%s' % original]
            if isinstance(area, list):
                start = func(area[0])
                new_area = [start, start + (area[1] - area[0])]
            else:
                new_area = func(area)

            hook['area_%s' % id] = new_area

    except:
        print('Key Error %s for %s (%s) for %s' % (error, hook['name'], hook['type'], id))

    #if hook['name'] == 'WM_onCreate': print(hook)


def do_module(src, dest):
    m = yaml.safe_load(open(src, 'r').read())

    if 'hooks' not in m:
        return

    for id, func in fix_for.items():
        for hook in m['hooks']:
            work_on_hook(hook, id, func)

    open(dest, 'w').write(yaml.dump(m))


def do_project(f, already_done):
    proj = yaml.safe_load(open(f, 'r').read())

    for m in proj['modules']:
        if m not in already_done:
            already_done.add(m)
            do_module(m.replace('processed/', ''), m)


def main():
    do_mapfile('kamek_pal.x', 'kamek_pal2.x', fix_offs_pal_v2)
    do_mapfile('kamek_pal.x', 'kamek_ntsc.x', fix_offs_ntsc_v1)
    do_mapfile('kamek_pal.x', 'kamek_ntsc2.x', fix_offs_ntsc_v2)
    do_mapfile('kamek_pal.x', 'kamek_jpn.x', fix_offs_jpn_v1)
    do_mapfile('kamek_pal.x', 'kamek_jpn2.x', fix_offs_jpn_v2)

    already_done = set()
    do_project('NewerProjectKP.yaml', already_done)

if __name__ == '__main__':
    main()


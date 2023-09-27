# Newer Super Mario Bros. Wii
# Custom Reggie! Next sprites.py Module
# By Kamek64, MalStar1000, RoadrunnerWMC


from PyQt5 import QtCore, QtGui
Qt = QtCore.Qt

import spritelib as SLib
import sprites_common as common

ImageCache = SLib.ImageCache

################################################################################
################################################################################
################################################################################

# Newer-specific base classes
class SpriteImage_NewerSwitch(common.SpriteImage_Switch):

    @staticmethod
    def loadImages():
        common.SpriteImage_Switch.loadImages()

        if 'QSwitch2' not in ImageCache:
            for i in range(2, 5):
                p = SLib.GetImg('q_switch%d.png' % i, True)

                if p is None:
                    # happens when the newer patch folder is not loaded yet
                    return

                ImageCache['QSwitch%d' % i] = QtGui.QPixmap.fromImage(p)
                ImageCache['QSwitchU%d' % i] = QtGui.QPixmap.fromImage(p.mirrored(True, True))

        if 'ESwitch2' not in ImageCache:
            for i in range(2, 5):
                p = SLib.GetImg('e_switch%d.png' % i, True)
                ImageCache['ESwitch%d' % i] = QtGui.QPixmap.fromImage(p)
                ImageCache['ESwitchU%d' % i] = QtGui.QPixmap.fromImage(p.mirrored(True, True))

    def dataChanged(self):
        self.styleType = self.parent.spritedata[3] & 0xF
        upsideDown = self.parent.spritedata[5] & 1

        if self.styleType > 0:
            self.yOffset = -16 + 32 * upsideDown
        else:
            self.yOffset = 0

        self.xOffset = 0

        if self.styleType == 1 or self.styleType > 4:
            self.styleType = 0

        # hack needed if data is changed before patch folder is loaded
        if 'QSwitch2' not in ImageCache:
            return

        super().dataChanged()

################################################################################
################################################################################
################################################################################

class SpriteImage_Block(SLib.SpriteImage):  # 207, 208, 209, 221, 255, 256, 402, 403, 422, 423
    def __init__(self, parent, scale=1.5):
        super().__init__(parent, scale)
        self.spritebox.shown = False

        self.tilenum = 1315
        self.contentsNybble = 5
        self.contentsOverride = None
        self.eightIsMushroom = False
        self.twelveIsMushroom = False
        self.rotates = False

    def dataChanged(self):
        super().dataChanged()

        # SET CONTENTS
        # In the blocks.png file:
        # 0 = Empty, 1 = Coin, 2 = Mushroom, 3 = Fire Flower, 4 = Propeller, 5 = Penguin Suit,
        # 6 = Mini Shroom, 7 = Star, 8 = Continuous Star, 9 = Yoshi Egg, 10 = 10 Coins,
        # 11 = 1-up, 12 = Vine, 13 = Spring, 14 = Shroom/Coin, 15 = Ice Flower, 16 = Toad, 17 = Hammer

        if self.contentsOverride is not None:
            contents = self.contentsOverride
        else:
            contents = self.parent.spritedata[self.contentsNybble] & 0xF

        if contents == 2:  # Hammer
            contents = 17

        if contents == 12 and self.twelveIsMushroom:
            contents = 2  # 12 is a mushroom on some types
        if contents == 8 and self.eightIsMushroom:
            contents = 2  # same as above, but for type 8

        self.image = ImageCache['BlockContents'][contents]

        # SET UP ROTATION
        if self.rotates:
            transform = QtGui.QTransform()
            transform.translate(12, 12)

            angle = (self.parent.spritedata[4] & 0xF0) >> 4
            leftTilt = self.parent.spritedata[3] & 1

            angle *= (45.0 / 16.0)

            if leftTilt == 0:
                transform.rotate(angle)
            else:
                transform.rotate(360.0 - angle)

            transform.translate(-12, -12)
            self.parent.setTransform(transform)

    def paint(self, painter):
        super().paint(painter)

        painter.setRenderHint(QtGui.QPainter.Antialiasing)
        if self.tilenum < len(SLib.Tiles):
            painter.drawPixmap(0, 0, SLib.Tiles[self.tilenum].main)
        painter.drawPixmap(0, 0, self.image)
        

class SpriteImage_QBlock(SpriteImage_Block):  # 207
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 49


class SpriteImage_QBlockUnused(SpriteImage_Block):  # 208
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 49
        self.eightIsMushroom = True
        self.twelveIsMushroom = True


class SpriteImage_BrickBlock(SpriteImage_Block):  # 209
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 48


class SpriteImage_RotatingQBlock(SpriteImage_Block):  # 255
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 49
        self.contentsNybble = 4
        self.twelveIsMushroom = True
        self.rotates = True


class SpriteImage_RotatingBrickBlock(SpriteImage_Block):  # 256
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 48
        self.contentsNybble = 4
        self.twelveIsMushroom = True
        self.rotates = True


class SpriteImage_LineQBlock(SpriteImage_Block):  # 402
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 49
        self.twelveIsMushroom = True


class SpriteImage_LineBrickBlock(SpriteImage_Block):  # 403
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.tilenum = 48


class SpriteImage_NewerGoomba(SLib.SpriteImage_StaticMultiple):  # 20
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Goomba', 'goomba.png')
        if 'Goomba1' in ImageCache: return
        for i in range(8):
            ImageCache['Goomba%d' % (i + 1)] = SLib.GetImg('goomba_%d.png' % (i + 1))

    def dataChanged(self):

        color = (self.parent.spritedata[2] & 0xF) % 9

        if color == 0:
            self.image = ImageCache['Goomba']
            self.offset = (-1, -4)
        else:
            self.image = ImageCache['Goomba%d' % color]
            self.offset = (0, -4) if color not in (7, 8) else (0, -5)

        super().dataChanged()


class SpriteImage_NewerParaGoomba(SLib.SpriteImage_StaticMultiple):  # 21
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('ParaGoomba', 'para_goomba.png')
        if 'ParaGoomba1' in ImageCache: return
        for i in range(8):
            ImageCache['ParaGoomba%d' % (i + 1)] = SLib.GetImg('para_goomba_%d.png' % (i + 1))

    def dataChanged(self):
        colour = (self.parent.spritedata[2] & 0xF) % 9
        self.offset = (-1, -10)

        if colour == 0:
            self.image = ImageCache['ParaGoomba']

        else:
            self.image = ImageCache['ParaGoomba%d' % colour]

        super().dataChanged()


class SpriteImage_NewerBabyGoomba(SLib.SpriteImage_StaticMultiple):  # 22
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('BabyGoomba', 'babygoomba.png')
        if 'BabyGoomba1' in ImageCache: return
        for i in range(8):
            ImageCache['BabyGoomba%d' % (i + 1)] = SLib.GetImg('babygoomba_%d.png' % (i + 1))

    def dataChanged(self):

        color = (self.parent.spritedata[2] & 0xF) % 9

        if color == 0:
            self.image = ImageCache['BabyGoomba']
            self.offset = (-1, -4)
        else:
            self.image = ImageCache['BabyGoomba%d' % color]
            self.offset = (0, -4) if color not in (7, 8) else (0, -5)

        super().dataChanged()


class SpriteImage_NewerBuzzyBeetle(SLib.SpriteImage_StaticMultiple):  # 24
    @staticmethod
    def loadImages():
        if "BuzzyBeetle" not in ImageCache:
            ImageCache["BuzzyBeetle"] = SLib.GetImg('buzzy_beetle.png')
            ImageCache["BuzzyBeetleU"] = SLib.GetImg('buzzy_beetle_u.png')
            ImageCache["BuzzyBeetleShell"] = SLib.GetImg('buzzy_beetle_shell.png')
            ImageCache["BuzzyBeetleShellU"] = SLib.GetImg('buzzy_beetle_shell_u.png')

        if "BuzzyBeetleBlack" not in ImageCache:
            for colour in ("Black", "Blue", "Green", "Orange", "Purple", "Red", "Yellow"):
                ImageCache["BuzzyBeetle%s" % colour] = SLib.GetImg('buzzy_beetle_%s.png' % colour.lower())
                ImageCache["BuzzyBeetle%sU" % colour] = SLib.GetImg('buzzy_beetle_%s_u.png' % colour.lower())
                ImageCache["BuzzyBeetle%sShell" % colour] = SLib.GetImg('buzzy_beetle_%s_shell.png' % colour.lower())
                ImageCache["BuzzyBeetle%sShellU" % colour] = SLib.GetImg('buzzy_beetle_%s_shell_u.png' % colour.lower())

    def dataChanged(self):

        orient = self.parent.spritedata[5] & 15
        colour = self.parent.spritedata[2] & 15
        if colour > 7:
            colour = 0

        colour = ("", "Red", "Orange", "Yellow", "Green", "Blue", "Purple", "Black")[colour]
        if orient == 1:
            self.image = ImageCache['BuzzyBeetle%sU' % colour]
            self.yOffset = 0
        elif orient == 2:
            self.image = ImageCache['BuzzyBeetle%sShell' % colour]
            self.yOffset = 2
        elif orient == 3:
            self.image = ImageCache['BuzzyBeetle%sShellU' % colour]
            self.yOffset = 2
        else:
            self.image = ImageCache['BuzzyBeetle%s' % colour]
            self.yOffset = 0

        super().dataChanged()


class SpriteImage_NewerSpiny(SLib.SpriteImage_StaticMultiple):  # 25
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Spiny', 'spiny.png')
        SLib.loadIfNotInImageCache('SpinyShell', 'spiny_shell.png')
        SLib.loadIfNotInImageCache('SpinyShellU', 'spiny_shell_u.png')
        SLib.loadIfNotInImageCache('SpinyBall', 'spiny_ball.png')

        if 'SpinyOrange' in ImageCache: return
        for colour in ("Orange", "Yellow", "Green", "Blue", "Violet", "Black", "Sidestepper"):
            ImageCache["Spiny%s" % colour] = SLib.GetImg('spiny_%s.png' % colour.lower())
            ImageCache["Spiny%sShell" % colour] = SLib.GetImg('spiny_%s_shell.png' % colour.lower())
            ImageCache["Spiny%sShellU" % colour] = SLib.GetImg('spiny_%s_shell_u.png' % colour.lower())

    def dataChanged(self):
        orient = self.parent.spritedata[5] & 15
        colour = self.parent.spritedata[2] & 15
        if colour > 7:
            colour = 0

        colour = ("", "Orange", "Yellow", "Green", "Blue", "Violet", "Black", "Sidestepper")[colour]
        if orient == 1:
            self.image = ImageCache['SpinyBall']
            self.yOffset = -2
        elif orient == 2:
            self.image = ImageCache['Spiny%sShell' % colour]
            self.yOffset = 1
        elif orient == 3:
            self.image = ImageCache['Spiny%sShellU' % colour]
            self.yOffset = 2
        else:
            self.image = ImageCache['Spiny%s' % colour]
            self.yOffset = 0

        super().dataChanged()


class SpriteImage_NewerUpsideDownSpiny(SLib.SpriteImage_StaticMultiple):  # 26
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('SpinyU', 'spiny_u.png')
        if 'SpinyOrangeU' not in ImageCache:
            for style in ("Orange", "Yellow", "Green", "Blue", "Violet", "Black", "Sidestepper"):
                Spiny = SLib.GetImg('spiny_%s.png' % style, True)
                
                if Spiny is None:
                    # slow patch folder is slow
                    return
            
                ImageCache['Spiny%sU' % style] = QtGui.QPixmap.fromImage(Spiny.mirrored(False, True))

    def dataChanged(self):
        colour = self.parent.spritedata[2] & 15
        if colour > 7:
            colour = 0

        colour = ("", "Orange", "Yellow", "Green", "Blue", "Violet", "Black", "Sidestepper")[colour]
        # slow patch folder is slow, 2
        if 'SpinyOrangeU' not in ImageCache:
            return
        
        self.image = ImageCache['Spiny%sU' % colour]
            
        super().dataChanged()


class SpriteImage_NewerQSwitch(SpriteImage_NewerSwitch): # 40
    def __init__(self, parent):
        super().__init__(parent)
        self.switchType = "Q"


class SpriteImage_NewerExcSwitch(SpriteImage_NewerSwitch):  # 42
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.switchType = 'E'


class SpriteImage_Thwomp(SLib.SpriteImage_StaticMultiple):  # 47
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['Thwomp'],
            (-6, -6),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Thwomp', 'thwomp.png')
        SLib.loadIfNotInImageCache('ThwompIce', 'thwomp_ice.png')

    def dataChanged(self):
        icy = self.parent.spritedata[2] & 1

        if icy:
            self.image = ImageCache['ThwompIce']
        else:
            self.image = ImageCache['Thwomp']


class SpriteImage_GiantThwomp(SLib.SpriteImage_StaticMultiple):  # 48
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['GiantThwomp'],
            (-8, -8),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('GiantThwomp', 'giant_thwomp.png')
        SLib.loadIfNotInImageCache('GiantThwompIce', 'giant_thwomp_ice.png')

    def dataChanged(self):
        icy = self.parent.spritedata[2] & 1

        if icy:
            self.image = ImageCache['GiantThwompIce']
        else:
            self.image = ImageCache['GiantThwomp']


class SpriteImage_NewerKoopa(SLib.SpriteImage_StaticMultiple):  # 57
    @staticmethod
    def loadImages():
        if 'KoopaG' in ImageCache: return
        ImageCache['KoopaG'] = SLib.GetImg('koopa_green.png')
        ImageCache['KoopaR'] = SLib.GetImg('koopa_red.png')
        ImageCache['KoopaShellG'] = SLib.GetImg('koopa_green_shell.png')
        ImageCache['KoopaShellR'] = SLib.GetImg('koopa_red_shell.png')
        for flag in (0, 1):
            for style in range(4):
                ImageCache['Koopa%d%d' % (flag, style + 1)] = \
                    SLib.GetImg('koopa_%d%d.png' % (flag, style + 1))
                if style < 3:
                    ImageCache['KoopaShell%d%d' % (flag, style + 1)] = \
                        SLib.GetImg('koopa_shell_%d%d.png' % (flag, style + 1))

    def dataChanged(self):
        # get properties
        props = self.parent.spritedata[5]
        shell = (props >> 4) & 1
        red = props & 1
        texhack = (self.parent.spritedata[2] & 0xF) % 5

        if not shell:

            if texhack == 0:
                self.offset = (-7, -15)
                self.image = ImageCache['KoopaG'] if not red else ImageCache['KoopaR']
            else:
                self.offset = (-5, -13)
                self.image = ImageCache['Koopa%d%d' % (red, texhack)]
        else:
            del self.offset

            if texhack in (0, 4):
                self.image = ImageCache['KoopaShellG'] if not red else ImageCache['KoopaShellR']
            else:
                self.image = ImageCache['KoopaShell%d%d' % (red, texhack)]

        super().dataChanged()


class SpriteImage_NewerParaKoopa(SLib.SpriteImage_StaticMultiple):  # 58
    def __init__(self, parent):
        super().__init__(parent, 1.5, None, (-7, -12))
        self.aux.append(SLib.AuxiliaryTrackObject(self.parent, 0, 0, 0))

    @staticmethod
    def loadImages():

        if 'ParaKoopaG' not in ImageCache:
            ImageCache['ParaKoopaG'] = SLib.GetImg('parakoopa_green.png')
            ImageCache['ParaKoopaR'] = SLib.GetImg('parakoopa_red.png')

        if 'KoopaShellG' not in ImageCache:
            ImageCache['KoopaShellG'] = SLib.GetImg('koopa_green_shell.png')
            ImageCache['KoopaShellR'] = SLib.GetImg('koopa_red_shell.png')

        if 'ParaKoopa01' not in ImageCache:
            for flag in (0, 1):
                for style in range(4):
                    ImageCache['ParaKoopa%d%d' % (flag, style + 1)] = \
                        SLib.GetImg('parakoopa_%d%d.png' % (flag, style + 1))

        if 'KoopaShell01' not in ImageCache:
            for flag in (0, 1):
                for style in range(4):
                    ImageCache['KoopaShell%d%d' % (flag, style + 1)] = \
                        SLib.GetImg('koopa_shell_%d%d.png' % (flag, style + 1))

    def dataChanged(self):
        # get properties
        red = self.parent.spritedata[5] & 1
        mode = (self.parent.spritedata[5] >> 4) & 3
        texhack = (self.parent.spritedata[2] & 0xF) % 5

        if texhack == 0:
            if mode == 3:
                del self.offset
                self.image = ImageCache['KoopaShellG'] if not red else ImageCache['KoopaShellR']
            else:
                self.offset = (-7, -12)
                self.image = ImageCache['ParaKoopaG'] if not red else ImageCache['ParaKoopaR']
        else:
            if mode == 3:
                del self.offset
                self.image = ImageCache['KoopaShell%d%d' % (red, texhack)]
            else:
                self.offset = (-8, -12)
                self.image = ImageCache['ParaKoopa%d%d' % (red, texhack)]
                
        if mode == 1 or mode == 2:

            turnImmediately = self.parent.spritedata[4] & 1 == 1
            track = self.aux[0]

            if mode == 1:
                track.direction = SLib.AuxiliaryTrackObject.Horizontal
                track.setSize(9 * 16, 16)
                if turnImmediately:
                    track.setPos(self.width / 2, self.height / 2)
                else:
                    track.setPos(-4 * 24 + self.width / 2, self.height / 2)
            else:
                track.direction = SLib.AuxiliaryTrackObject.Vertical
                track.setSize(16, 9 * 16)
                if turnImmediately:
                    track.setPos(self.width / 2, self.height / 2)
                else:
                    track.setPos(self.width / 2, -4 * 24 + self.height / 2)

        else:
            # hide the track
            self.aux[0].setSize(0, 0)

        super().dataChanged()


class SpriteImage_NewerSpikeTop(SLib.SpriteImage_StaticMultiple):  # 60
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            offset = (0, -4)
        )

    @staticmethod
    def loadImages():
        if 'SpikeTop00' in ImageCache: return
        for style in ("", "_Red", "_Orange", "_Yellow", "_Green", "_Hothead", "_Purple", "_Black"):
            SpikeTop = SLib.GetImg('spiketop%s.png' % style, True)
            if SpikeTop is None:
                # happens when the newer patch folder is not loaded yet
                return

            Transform = QtGui.QTransform()
            ImageCache['SpikeTop00%s' % style] = QtGui.QPixmap.fromImage(SpikeTop.mirrored(True, False))
            Transform.rotate(90)
            ImageCache['SpikeTop10%s' % style] = ImageCache['SpikeTop00%s' % style].transformed(Transform)
            Transform.rotate(90)
            ImageCache['SpikeTop20%s' % style] = ImageCache['SpikeTop00%s' % style].transformed(Transform)
            Transform.rotate(90)
            ImageCache['SpikeTop30%s' % style] = ImageCache['SpikeTop00%s' % style].transformed(Transform)

            Transform = QtGui.QTransform()
            ImageCache['SpikeTop01%s' % style] = QtGui.QPixmap.fromImage(SpikeTop)
            Transform.rotate(90)
            ImageCache['SpikeTop11%s' % style] = ImageCache['SpikeTop01%s' % style].transformed(Transform)
            Transform.rotate(90)
            ImageCache['SpikeTop21%s' % style] = ImageCache['SpikeTop01%s' % style].transformed(Transform)
            Transform.rotate(90)
            ImageCache['SpikeTop31%s' % style] = ImageCache['SpikeTop01%s' % style].transformed(Transform)

    def dataChanged(self):
        orientation = (self.parent.spritedata[5] >> 4) % 4
        direction = self.parent.spritedata[5] & 1
        colour = self.parent.spritedata[2] & 7
        
        color = ("", "_Red", "_Orange", "_Yellow", "_Green", "_Hothead", "_Purple", "_Black")[colour]
        # slow patch folder is slow, 2
        if 'SpikeTop01_Red' not in ImageCache:
            return
        
        self.image = ImageCache['SpikeTop%d%d%s' % (orientation, direction, color)]

        if colour == 5:
            self.offset = (
                (0, -4),
                (-4, 0),
                (0, -4),
                (-4, 0),
            )[orientation]
        else:
            self.offset = (
                (0, -4),
                (0, 0),
                (0, 0),
                (-4, 0),
            )[orientation]

        super().dataChanged()


class SpriteImage_NewerSpikeBall(SLib.SpriteImage_StaticMultiple):  # 63
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['SpikeBall'],
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('SpikeBall', 'spike_ball.png')
        SLib.loadIfNotInImageCache('SnowBall', 'snow_ball.png')

    def dataChanged(self):
        snowy = self.parent.spritedata[2] & 1
        if snowy:
            self.image = ImageCache['SnowBall']
        else:
            self.image = ImageCache['SpikeBall']


class SpriteImage_NewerBouncyCloud(SLib.SpriteImage_StaticMultiple):  # 78
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['CloudTrSmall'],
            (-2, -2),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('CloudTrBig', 'cloud_trampoline_big.png')
        SLib.loadIfNotInImageCache('CloudTrSmall', 'cloud_trampoline_small.png')

        if 'CloudTrBig1' in ImageCache:
            return

        for size in ("Big", "Small"):
            for style in range(1, 7):
                name = "CloudTr%s%d" % (size, style)
                ImageCache[name] = SLib.GetImg("cloud_trampoline_%s%d.png" % (size.lower(), style))

    def dataChanged(self):
        style = self.parent.spritedata[2] & 0xF
        raw_size = (self.parent.spritedata[4] >> 4) & 1
        size = "Small" if raw_size == 0 else "Big"

        if style == 0 or style > 7:
            self.image = ImageCache['CloudTr%s' % size]
            self.offset = (-2, -2)
        else:
            self.image = ImageCache['CloudTr%s%d' % (size, style)]
            self.offset = (-2, -2)

        super().dataChanged()


class SpriteImage_GiantSpikeBall(SLib.SpriteImage_StaticMultiple):  # 98
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['GiantSpikeBall'],
            (-24, -24),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('GiantSpikeBall', 'giant_spike_ball.png')
        SLib.loadIfNotInImageCache('GiantSnowBall', 'giant_snow_ball.png')

    def dataChanged(self):
        snowy = self.parent.spritedata[2] & 1

        if snowy:
            self.image = ImageCache['GiantSnowBall']
        else:
            self.image = ImageCache['GiantSpikeBall']


class SpriteImage_NewerBobomb(SLib.SpriteImage_StaticMultiple):  # 101
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Bobomb', 'bobomb.png')
        if 'Bobomb1' in ImageCache: return
        for i in range(8):
            ImageCache['Bobomb%d' % (i + 1)] = SLib.GetImg('bobomb_%d.png' % (i + 1))
        
    def dataChanged(self):
        colour = (self.parent.spritedata[2] & 0xF) % 9
        if colour == 0:
            self.image = ImageCache['Bobomb']
            self.offset = (-8, -8)
        else:
            self.image = ImageCache['Bobomb%d' % colour]
            self.offset = (-10, -8)

        super().dataChanged()


class SpriteImage_NewerFloatingBarrel(SLib.SpriteImage_StaticMultiple):  # 145
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            offset = (-16, -9)
        )
        image = ImageCache['FloatingBarrel']
        self.width = (image.width() / self.scale) + 1
        self.height = (image.height() / self.scale) + 2
        
        self.aux.append(SLib.AuxiliaryImage(parent, image.width(), image.height()))
        self.aux.append(SLib.AuxiliaryRectOutline(parent, 72, 2, 0, 36))
    
    @staticmethod
    def loadImages():
        if 'FloatingBarrel' in ImageCache: return
        ImageCache['FloatingBarrel'] = SLib.GetImg('barrel_floating.png')
        for flag in (1, 2):
            ImageCache['FloatingBarrel%d' % flag] = SLib.GetImg('barrel_floating_%d.png' % flag)

    def dataChanged(self):
        color = self.parent.spritedata[2] & 3
        if color == 0:
            img = ImageCache['FloatingBarrel']
        else:
            img = ImageCache['FloatingBarrel%d' % color]
        
        self.aux[0].image = img
            
        # Don't let SLib.SpriteImage_Static reset size
        SLib.SpriteImage.dataChanged(self)


class SpriteImage_MessageBlock(SLib.SpriteImage_Static): # 152
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            SLib.Tiles[0x98].main,
            (8, 0)
        )


class SpriteImage_NewerQSwitchUnused(SpriteImage_NewerSwitch): # 153
    def __init__(self, parent):
        super().__init__(parent)
        self.switchType = "Q"


class SpriteImage_Meteor(SLib.SpriteImage_StaticMultiple):  # 183
    @staticmethod
    def loadImages():
        ImageCache['Meteor'] = SLib.GetImg('meteor.png')
        ImageCache['MeteorElectric'] = SLib.GetImg('meteor_electric.png')

    def dataChanged(self):
        multiplier = self.parent.spritedata[4] / 20
        if multiplier == 0: multiplier = 0.01
        isElectric = (self.parent.spritedata[5] >> 4) & 1

        # Get the size data, taking into account the size
        # differences between the non-electric and
        # electric varieties.
        sizes = (
            # Relative X offset (size 0x14),
            # relative Y offset (size 0x14),
            # absolute X offset,
            # absolute Y offset
            (-54, -53, 6, -1),
            (-61, -68, 6, 0),
        )
        size = sizes[1] if isElectric else sizes[0]
        relXoff = size[0]
        relYoff = size[1]
        absXoff = size[2]
        absYoff = size[3]

        base = ImageCache['MeteorElectric' if isElectric else 'Meteor']
        if base is None:
            return

        self.image = base.scaled(
            int(base.width() * multiplier) + 8,
            int(base.height() * multiplier) + 8,
        )
        self.offset = (
            (relXoff * multiplier) + absXoff,
            (relYoff * multiplier) + absYoff,
        )

        super().dataChanged()


class SpriteImage_MidwayFlag(SLib.SpriteImage_StaticMultiple):  # 188
    def __init__(self, parent):
        super().__init__(parent)
        self.yOffset = -37

    @staticmethod
    def loadImages():
        if 'MidwayFlag0' in ImageCache: return
        for i in range(18):
            ImageCache['MidwayFlag%d' % i] = SLib.GetImg('midway_flag_%d.png' % i)

    def dataChanged(self):

        style = self.parent.spritedata[2]
        if style > 17: style = 0

        self.image = ImageCache['MidwayFlag%d' % style]

        super().dataChanged()


class SpriteImage_TileEventNewer(common.SpriteImage_TileEvent):  # 191
    def __init__(self, parent):
        super().__init__(parent)
        self.notAllowedTypes = (2, 5, 7, 13, 15)

    def getTileFromType(self, type_):
        if type_ == 0:
            return SLib.GetTile(55)

        if type_ == 1:
            return SLib.GetTile(48)

        if type_ == 3:
            return SLib.GetTile(52)

        if type_ == 4:
            return SLib.GetTile(51)

        if type_ == 6:
            return SLib.GetTile(45)

        if type_ in [8, 9, 10, 11]:
            row = self.parent.spritedata[2] & 0xF
            col = self.parent.spritedata[3] >> 4

            tilenum = 256 * (type_ - 8)
            tilenum += row * 16 + col

            return SLib.GetTile(tilenum)

        if type_ == 12:
            return SLib.GetTile(256 * 3 + 67)

        if type_ == 14:
            return SLib.GetTile(256)

        return None
        

class SpriteImage_NewerHuckitCrab(SLib.SpriteImage_StaticMultiple):  # 195
    @staticmethod
    def loadImages():
        if 'HuckitCrabWhiteR' in ImageCache: return
        Huckitcrab = SLib.GetImg('huckit_crab.png', True)
        Wintercrab = SLib.GetImg('huckit_crab_white.png', True)
        if Wintercrab is None:
            return
        
        ImageCache['HuckitCrabL'] = QtGui.QPixmap.fromImage(Huckitcrab)
        ImageCache['HuckitCrabR'] = QtGui.QPixmap.fromImage(Huckitcrab.mirrored(True, False))
        ImageCache['HuckitCrabWhiteL'] = QtGui.QPixmap.fromImage(Wintercrab)
        ImageCache['HuckitCrabWhiteR'] = QtGui.QPixmap.fromImage(Wintercrab.mirrored(True, False))

    def dataChanged(self):
        info = self.parent.spritedata[5]
        colour = self.parent.spritedata[2] & 1

        colour = ("", "White")[colour]
        if 'HuckitCrabWhiteR' not in ImageCache:
            return
        if info == 1:
            self.image = ImageCache['HuckitCrab%sR' % colour]
            self.xOffset = 0
        else:
            if info == 13:
                self.image = ImageCache['HuckitCrab%sR' % colour]
                self.xOffset = 0
            else:
                self.image = ImageCache['HuckitCrab%sL' % colour]
                self.xOffset = -16

        super().dataChanged()


class SpriteImage_NewerGiantGoomba(SLib.SpriteImage_StaticMultiple):  # 198
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('GiantGoomba', 'giant_goomba.png')
        if 'GiantGoomba1' in ImageCache: return
        for i in range(6):
            ImageCache['GiantGoomba%d' % (i + 1)] = SLib.GetImg('giant_goomba_%d.png' % (i + 1))

    def dataChanged(self):

        colour = (self.parent.spritedata[2] & 0xF) % 7

        if colour == 0:
            self.image = ImageCache['GiantGoomba']
            self.offset = (-6, -19)
        else:
            self.image = ImageCache['GiantGoomba%d' % colour]
            self.offset = (-7, -18)

        super().dataChanged()
        

class SpriteImage_NewerMegaGoomba(SLib.SpriteImage_StaticMultiple):  # 198
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('MegaGoomba', 'mega_goomba.png')
        if 'MegaGoomba1' in ImageCache: return
        for i in range(6):
            ImageCache['MegaGoomba%d' % (i + 1)] = SLib.GetImg('mega_goomba_%d.png' % (i + 1))

    def dataChanged(self):
        colour = (self.parent.spritedata[2] & 0xF) % 7

        if colour == 0:
            self.image = ImageCache['MegaGoomba']
            self.offset = (-11, -37)
        else:
            self.image = ImageCache['MegaGoomba%d' % colour]
            self.offset = (-11, -37)

        super().dataChanged()


class SpriteImage_Topman(SLib.SpriteImage_Static):  # 210
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['Topman'],
            (-22, -32),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Topman', 'topman.png')


class SpriteImage_NewerSpringBlock(SLib.SpriteImage_StaticMultiple):  # 223
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('SpringBlock', 'spring_block.png')
        SLib.loadIfNotInImageCache('SpringBlockAlt', 'spring_block_alt.png')
        if 'SpringBlock1' in ImageCache: return
        for i in range(3):
            ImageCache['SpringBlock%d' % (i + 1)] = SLib.GetImg('spring_block_%d.png' % (i + 1))
            ImageCache['SpringBlockAlt%d' % (i + 1)] = SLib.GetImg('spring_block_alt_%d.png' % (i + 1))

    def dataChanged(self):
        colour = (self.parent.spritedata[2] & 0xF) % 4
        alt = self.parent.spritedata[5] & 1
        
        if colour == 0:
            self.image = ImageCache['SpringBlockAlt'] if alt else ImageCache['SpringBlock']
        else:
            self.image = ImageCache['SpringBlockAlt%d' % colour] if alt else ImageCache['SpringBlock%d' % colour]

        super().dataChanged()


class SpriteImage_NewerBramball(SLib.SpriteImage_StaticMultiple):  # 230
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Bramball', 'bramball.png')
        if 'Bramball1' in ImageCache: return
        for i in range(6):
            ImageCache['Bramball%d' % (i + 1)] = SLib.GetImg('bramball_%d.png' % (i + 1))

    def dataChanged(self):
        colour = (self.parent.spritedata[2] & 0xF) % 5

        if colour == 0:
            self.image = ImageCache['Bramball']
            self.offset = (-32, -48)
        else:
            self.image = ImageCache['Bramball%d' % colour]
            self.offset = (-33, -47)

        super().dataChanged()


class SpriteImage_NewerWiggleShroom(SLib.SpriteImage):  # 231
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.spritebox.shown = False

    @staticmethod
    def loadImages():
        if 'WiggleShroomL' in ImageCache: return
        ImageCache['WiggleShroomL'] = SLib.GetImg('wiggle_shroom_left.png')
        ImageCache['WiggleShroomM'] = SLib.GetImg('wiggle_shroom_middle.png')
        ImageCache['WiggleShroomR'] = SLib.GetImg('wiggle_shroom_right.png')
        ImageCache['WiggleShroomS'] = SLib.GetImg('wiggle_shroom_stem.png')
        
        if 'WiggleShroomRedL' in ImageCache: return
        for style in ("Red", "Orange", "Green", "Blue"):
            ImageCache['WiggleShroom%sL' % (style)] = SLib.GetImg('wiggle_shroom_%s_left.png' % (style))
            ImageCache['WiggleShroom%sM' % (style)] = SLib.GetImg('wiggle_shroom_%s_middle.png' % (style))
            ImageCache['WiggleShroom%sR' % (style)] = SLib.GetImg('wiggle_shroom_%s_right.png' % (style))
            ImageCache['WiggleShroomNewerS'] = SLib.GetImg('wiggle_shroom_newer_stem.png')

    def dataChanged(self):
        super().dataChanged()
        
        width = (self.parent.spritedata[4] & 0xF0) >> 4
        stemlength = self.parent.spritedata[3] & 3
        colour = (self.parent.spritedata[2] & 0xF) % 6
        shroom = ("", "Red", "Orange", "", "Green", "Blue")[colour]
        
        self.wiggleleft = ImageCache['WiggleShroom%sL' % shroom]
        self.wigglemiddle = ImageCache['WiggleShroom%sM' % shroom]
        self.wiggleright = ImageCache['WiggleShroom%sR' % shroom]
        if colour > 0:
            self.wigglestem = ImageCache['WiggleShroomNewerS']
        else:
            self.wigglestem = ImageCache['WiggleShroomS']
            
        self.xOffset = -(width * 8) - 20
        self.width = (width * 16) + 56
        self.height = (stemlength * 16) + 64

        self.parent.setZValue(24999)

    def paint(self, painter):
        super().paint(painter)
        
        xsize = self.width * 1.5
        painter.drawPixmap(0, 0, self.wiggleleft)
        painter.drawTiledPixmap(18, 0, xsize - 36, 24, self.wigglemiddle)
        painter.drawPixmap(xsize - 18, 0, self.wiggleright)
        painter.drawTiledPixmap((xsize / 2) - 12, 24, 24, (self.height * 1.5) - 24, self.wigglestem)


class SpriteImage_EventBlock(SLib.SpriteImage_Static): # 239
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            SLib.Tiles[0x97].main,
            (-8, -16)
        )


class SpriteImage_LineEvent(SLib.SpriteImage):  # 244
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.aux.append(SLib.AuxiliaryRectOutline(parent, 0, 0))

    def dataChanged(self):
        super().dataChanged()
        width = (self.parent.spritedata[5] >> 4) & 0xF
        height = self.parent.spritedata[4] & 0xF
        if width == 0:
            w = 1
        else:
            w = 0
        if height == 0:
            h = 1
        else:
            h = 0
        
        self.aux[0].setSize((width + w) * 24, (height + h) * 24)


class SpriteImage_ElectricLine(SLib.SpriteImage_StaticMultiple):  # 250
    def __init__(self, parent):
        super().__init__(parent)
        self.aux.append(SLib.AuxiliaryRectOutline(parent, 48, 672))
        self.aux[0].fillFlag = False

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('ElectricLineLeft', 'electric_line_left.png')
        SLib.loadIfNotInImageCache('ElectricLineRight', 'electric_line_right.png')

    def dataChanged(self):
        left = self.parent.spritedata[5] & 1

        if left:
            self.image = ImageCache['ElectricLineLeft']
            self.offset = (-16, -128)
            self.aux[0].setPos(24, -144)
        else:
            self.image = ImageCache['ElectricLineRight']
            self.offset = (-8, -128)
            self.aux[0].setPos(-12, -144)

        super().dataChanged()


class SpriteImage_Thundercloud(SLib.SpriteImage_Static):  # 251
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['Thundercloud'],
            (-24, -40),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Thundercloud', 'thundercloud.png')


class SpriteImage_NewerParabomb(SLib.SpriteImage_StaticMultiple):  # 269
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Parabomb', 'parabomb.png')
        if 'Parabomb1' in ImageCache: return
        for i in range(8):
            ImageCache['Parabomb%d' % (i + 1)] = SLib.GetImg('parabomb_%d.png' % (i + 1))

    def dataChanged(self):
        colour = (self.parent.spritedata[2] & 0xF) % 9
        self.offset = (-2, -16)

        if colour == 0:
            self.image = ImageCache['Parabomb']
            
        else:
            self.image = ImageCache['Parabomb%d' % colour]

        super().dataChanged()


class SpriteImage_AngrySun(SLib.SpriteImage_StaticMultiple):  # 282
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('AngrySun', 'angry_sun.png')
        SLib.loadIfNotInImageCache('AngryMoon', 'angry_moon.png')

    def dataChanged(self):
        isMoon = self.parent.spritedata[5] & 1
        self.image = ImageCache['AngrySun' if not isMoon else 'AngryMoon']
        self.offset = (-18, -18) if not isMoon else (-13, -13)

        super().dataChanged()


class SpriteImage_NewerMegaBuzzy(SLib.SpriteImage_StaticMultiple):  # 479
    @staticmethod
    def loadImages():
        if 'MegaBuzzyR' in ImageCache: return
        buzz = SLib.GetImg('megabuzzy.png', True)
        ImageCache['MegaBuzzyR'] = QtGui.QPixmap.fromImage(buzz)
        ImageCache['MegaBuzzyL'] = QtGui.QPixmap.fromImage(buzz.mirrored(True, False))
        SLib.loadIfNotInImageCache('MegaBuzzyF', 'megabuzzy_front.png')
        
        if 'MegaBuzzyRedR' in ImageCache: return
        for style in ("Red", "Orange", "Yellow", "Green", "Blue", "Purple", "Black", "ShyGuy", "Monty"):
            buzzy = SLib.GetImg('megabuzzy_%s.png' % style, True)
            if buzzy is None: return
            ImageCache['MegaBuzzy%sR' % style] = QtGui.QPixmap.fromImage(buzzy)
            ImageCache['MegaBuzzy%sL' % style] = QtGui.QPixmap.fromImage(buzzy.mirrored(True, False))
            ImageCache['MegaBuzzy%sF' % style] = SLib.GetImg('megabuzzy_%s_front.png' % style)

    def dataChanged(self):
    
        direction = self.parent.spritedata[5] & 4
        style = self.parent.spritedata[2] & 10
        dir = ("R", "L", "F", "R")[direction]
        colour = ("", "Red", "Orange", "Yellow", "Green", "Blue", "Purple", "Black", "ShyGuy", "Monty")[style]
        
        if 'MegaBuzzyShyGuyF' not in ImageCache: return
        self.image = ImageCache['MegaBuzzy%s%s' % (colour, dir)]
        
        if style == 8: # Shy Guy's offset
            if direction == 2:
                self.offset = (-31, -71)
            else:
                self.offset = (-36, -71)
        elif style == 9: # Monty Mole's offset
            if direction == 2:
                self.offset = (-49, -73)
            else:
                self.offset = (-40, -73)
        else: # Buzzy Beetle's offset
            if direction == 2:
                self.offset = (-39, -74)
            else:
                self.offset = (-43, -74)


        super().dataChanged()


class SpriteImage_Flipblock(SLib.SpriteImage_Static):  # 319
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['Flipblock'],
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Flipblock', 'flipblock.png')


class SpriteImage_NewerBigShell(SLib.SpriteImage_StaticMultiple):  # 341
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.offset = (-97, -145)

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('BigShellGreen', 'bigshell_green.png')
        SLib.loadIfNotInImageCache('BigShellGreenGrass', 'bigshell_green_grass.png')
        SLib.loadIfNotInImageCache('BigShellRed', 'bigshell_red.png')
        SLib.loadIfNotInImageCache('BigShellRedGrass', 'bigshell_red_grass.png')

    def dataChanged(self):
        style = self.parent.spritedata[5] & 1
        colour = self.parent.spritedata[2] & 1
        
        colour = ("Green", "Red")[colour]
        if style == 0:
            self.image = ImageCache['BigShell%sGrass' % colour]
        else:
            self.image = ImageCache['BigShell%s' % colour]

        super().dataChanged()


class SpriteImage_ShyGuy(SLib.SpriteImage_StaticMultiple):  # 351
    @staticmethod
    def loadImages():
        if 'ShyGuy0' in ImageCache: return
        for i in range(9):  # 0-8
            if i == 7: continue  # there's no ShyGuy7.png
            ImageCache['ShyGuy%d' % i] = SLib.GetImg('shyguy_%d.png' % i)

    def dataChanged(self):
        type = (self.parent.spritedata[2] >> 4) % 9

        imgtype = type if type != 7 else 6  # both linear ballooneers have image 6
        self.image = ImageCache['ShyGuy%d' % imgtype]

        self.offset = (
            (6, -7),  # 0: red
            (6, -7),  # 1: blue
            (6, -4),  # 2: red (sleeper)
            (7, -6),  # 3: yellow (jumper)
            (6, -8),  # 4: purple (judo)
            (6, -8),  # 5: green (spike thrower)
            (2, -9),  # 6: red (ballooneer - vertical)
            (2, -9),  # 7: red (ballooneer - horizontal)
            (2, -9),  # 8: blue (ballooneer - circular)
        )[type]

        super().dataChanged()


class SpriteImage_NewerGlowBlock(SLib.SpriteImage):  # 391
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.spritebox.shown = False

        self.aux.append(SLib.AuxiliaryImage(parent, 48, 48))
        self.aux[0].setPos(-12, -12)
    
    def dataChanged(self):
        purple = self.parent.spritedata[2] & 1
        self.aux[0].image = ImageCache['GlowBlock' if not purple else 'GlowBlockPurple']

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('GlowBlock', 'glow_block.png')
        SLib.loadIfNotInImageCache('GlowBlockPurple', 'glow_block_1.png')


class SpriteImage_GigaGoomba(SLib.SpriteImage_Static):  # 410
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['GigaGoomba'],
            (-108, -160),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('GigaGoomba', 'goomba_giga.png')


class SpriteImage_NewerBowserSwitchSm(SpriteImage_NewerSwitch):  # 478
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.switchType = 'E'


class SpriteImage_NewerBowserSwitchLg(SLib.SpriteImage_StaticMultiple):  # 479
    @staticmethod
    def loadImages():
        if 'ELSwitch' in ImageCache: return
        elg = SLib.GetImg('e_switch_lg.png', True)
        ImageCache['ELSwitch'] = QtGui.QPixmap.fromImage(elg)
        ImageCache['ELSwitchU'] = QtGui.QPixmap.fromImage(elg.mirrored(True, True))
        
        if 'ELSwitch1' not in ImageCache:
            for i in range(1, 6):
                elg2 = SLib.GetImg('e_switch_lg%d.png' % i, True)
            
                if elg2 is None: 
                    return
                
                ImageCache['ELSwitch%d' % i] = QtGui.QPixmap.fromImage(elg2)
                ImageCache['ELSwitchU%d' % i] = QtGui.QPixmap.fromImage(elg2.mirrored(True, True))

    def dataChanged(self):
    
        colour = (self.parent.spritedata[3] & 0xF) % 6
        upsideDown = self.parent.spritedata[5] & 1
        
        if 'ELSwitch5' not in ImageCache: return
        if colour == 0:
            if not upsideDown:
                self.image = ImageCache['ELSwitch']
                self.offset = (-15, -24)
            else:
                self.image = ImageCache['ELSwitchU']
                self.offset = (-15, 0)
        else:
            if not upsideDown:
                self.image = ImageCache['ELSwitch%d' % colour]
                self.offset = (-15, -40)
            else:
                self.image = ImageCache['ELSwitchU%d' % colour]
                self.offset = (-15, -16)


        super().dataChanged()


class SpriteImage_ShyGuyGiant(SLib.SpriteImage_Static): # 167
    def __init__(self, parent):
        super().__init__(parent, 1.5)

    @staticmethod
    def loadImages():
        if "ShyGuy%s%s" in ImageCache: return
        for size in ("big", "mega", "colossal"):
            for colour in ("red", "blue", "green", "yellow", "magenta"):
                ImageCache['ShyGuy%s%s' % (size, colour)] = SLib.GetImg('shyguy_%s_%s.png' % (size, colour))

    def dataChanged(self):
        size = (self.parent.spritedata[2] >> 4) & 3
        colour = (self.parent.spritedata[2] & 0xF) % 5
        scale = ("big", "mega", "colossal")[size]
        color = ("red", "blue", "green", "yellow", "magenta")[colour]

        self.image = ImageCache['ShyGuy%s%s' % (scale, color)]

        if size == 0:
            self.offset = (-12.7, -124)
        elif size == 1:
            self.offset = (-32, -165.3)
        else:
            self.offset = (-52.7, -229.3)

        super().dataChanged()


class SpriteImage_DragonCoasterPiece(SLib.SpriteImage_StaticMultiple): # 18
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5
        )

        self.yOffset = -4

    @staticmethod
    def loadImages():
        if 'DragonHead' in ImageCache: return
        ImageCache['DragonHead'] = SLib.GetImg('dragon_coaster_head.png')
        ImageCache['DragonBody'] = SLib.GetImg('dragon_coaster_body.png')
        ImageCache['DragonTail'] = SLib.GetImg('dragon_coaster_tail.png')

    def dataChanged(self):
        piece = self.parent.spritedata[5] & 3
        direction = (self.parent.spritedata[3] >> 4) & 1
        rotates = self.parent.spritedata[2] & 0x10

        sPiece = ("Head", "Body", "Tail")[piece]

        self.image = ImageCache["Dragon%s" % sPiece]

        transform = None

        if direction == 1:
            transform = QtGui.QTransform()
            transform.translate(12, 0)
            transform.scale(-1, 1)
            transform.translate(-12, 0)
        else:
            self.xOffset = -16

        if rotates:
            if transform is None:
                transform = QtGui.QTransform()

            angle = self.parent.spritedata[2] & 0xF

            if angle < 8:
                angle -= 8
            else:
                angle -= 7

            angle *= (180.0 / 16)

            transform.translate(24, 15)
            transform.rotate(angle)
            transform.translate(-24, -15)

        if transform is not None:
            self.parent.setTransform(transform)

        super().dataChanged()


class SpriteImage_FakeStarCoin(SLib.SpriteImage_Static):  # 49
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['FakeStarCoin'],
            (-8, -16),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('FakeStarCoin', 'starcoin_fake.png')

class SpriteImage_ClownCar(SLib.SpriteImage_Static):  # 13
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['ClownCar'],
            (16, -28),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('ClownCar', 'clown_car.png')

class SpriteImage_StarCollectable(SLib.SpriteImage_Static):  # 12
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['StarCollectable'],
            (3, 3),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('StarCollectable', 'star_collectable.png')


class SpriteImage_PumpkinGoomba(SLib.SpriteImage_StaticMultiple):  # 22
    def __init__(self, parent):
        super().__init__(parent)
        self.offset = (-4, -48)

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('PumpkinGoomba', 'pumpkin_goomba.png')
        SLib.loadIfNotInImageCache('PumpkinParagoomba', 'pumpkin_paragoomba.png')

    def dataChanged(self):
        para = self.parent.spritedata[5] & 1
        self.image = ImageCache['PumpkinGoomba' if not para else 'PumpkinParagoomba']

        super().dataChanged()

class SpriteImage_ActorSpawner(SLib.SpriteImage_Static):  # 88
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['ActorSpawner'],
            (8, 0),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('ActorSpawner', 'ActorSpawner.png')


class SpriteImage_ActorMultiSpawner(SLib.SpriteImage_Static):  # 89
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['ActorMultiSpawner'],
            (-8, -16),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('ActorMultiSpawner', 'ActorMultiSpawner.png')

class SpriteImage_FallingChestnut(SLib.SpriteImage_StaticMultiple):  # 320
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('ChestnutBrown', 'chestnut_brown.png')
        SLib.loadIfNotInImageCache('ChestnutYellow', 'chestnut_yellow.png')

    def dataChanged(self):
        yellow = self.parent.spritedata[5] & 1
        scale = (self.parent.spritedata[5] >> 4) & 0xF
        color = ("Brown", "Yellow")[yellow]

        if ImageCache['ChestnutBrown'] is None: return
        self.image = ImageCache['Chestnut%s' % color].scaled(*(
            (45, 40),
            (68, 59),
            (92, 80),
            (116, 99),
            (139, 119),
            (163, 140),
            (185, 159),
            (209, 180),
            (233, 199),
            (255, 219),
            (279, 238),
            (303, 258),
            (325, 278),
            (349, 298),
            (372, 318),
            (395, 338)
        )[scale])
        self.offset = (
                (-6, -11),#divide by 1.5
                (-14, -26),
                (-22, -41),
                (-30, -53),
                (-37, -67),
                (-46, -81),
                (-53, -96),
                (-61, -110),
                (-69, -124),
                (-77, -137),
                (-85, -151),
                (-93, -165),
                (-101, -178),
                (-109, -193),
                (-117, -207),
                (-125, -221)
            )[scale]

        super().dataChanged()

class SpriteImage_FuzzyBear(SLib.SpriteImage_StaticMultiple):  # 515
    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('FuzzyBear', 'fuzzy_bear.png')
        SLib.loadIfNotInImageCache('FuzzyBearBig', 'fuzzy_bear_big.png')

    def dataChanged(self):
        big = (self.parent.spritedata[2] >> 4) & 1
        self.image = ImageCache['FuzzyBear' if not big else 'FuzzyBearBig']

        super().dataChanged()

class SpriteImage_MegaThwomp(SLib.SpriteImage):  # 322
    def __init__(self, parent):
        super().__init__(parent)
        self.aux.append(SLib.AuxiliaryImage(parent, 121, 140))
        self.aux[0].image = ImageCache['MegaThwomp']
        self.aux[0].setPos(-60, -101)

        self.aux.append(SLib.AuxiliaryTrackObject(parent, 16, 16, SLib.AuxiliaryTrackObject.Horizontal))
        self.aux.append(SLib.AuxiliaryTrackObject(parent, 16, 16, SLib.AuxiliaryTrackObject.Horizontal))
        self.aux.append(SLib.AuxiliaryTrackObject(parent, 16, 16, SLib.AuxiliaryTrackObject.Vertical))

    def dataChanged(self):
        super().dataChanged()

        left_buffer = self.parent.spritedata[2] + 2
        right_buffer = self.parent.spritedata[3] + 2
        top_buffer = self.parent.spritedata[4] + 2

        self.aux[1].setSize(left_buffer * 8, 16)
        self.aux[1].setPos((-left_buffer * 12) + 24, 0)

        self.aux[2].setSize(right_buffer * 8, 16)
        self.aux[2].setPos(0, 0)

        self.aux[3].setSize(16, top_buffer * 8)
        self.aux[3].setPos(0, (-top_buffer * 12) + 24)

        if left_buffer == 2:
            self.aux[1].setSize(0, 0)

        if right_buffer == 2:
            self.aux[2].setSize(0, 0)

        if top_buffer == 2:
            self.aux[3].setSize(0, 0)

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('MegaThwomp', 'giant_thwomp.png')

class SpriteImage_Boolossus(SLib.SpriteImage_Static):  # 290
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['Boolossus'],
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('Boolossus', 'boolossus.png')

class SpriteImage_RockyBoss(SLib.SpriteImage_Static):  # 279
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['RockyBoss'],
            (-25, -33),
        )

        self.aux.append(SLib.AuxiliaryRectOutline(parent, 71, 77, -166, -50))
        self.aux.append(SLib.AuxiliaryRectOutline(parent, 71, 77, -334, -2))

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('RockyBoss', 'rocky_boss.png')

class SpriteImage_RockyBossWrench(SLib.SpriteImage_StaticMultiple):  # 302
    @staticmethod
    def loadImages():
        if 'RockyBossWrench0' in ImageCache: return
        for big in (0, 1):
            for direction in range(3):
                ImageCache['RockyBossWrench%d%d' % (big, direction)] = SLib.GetImg('rocky_boss_wrench_%d%d.png' % (big, direction))

    def dataChanged(self):
        big = (self.parent.spritedata[5] >> 4) & 1
        direction = self.parent.spritedata[5] & 3
        if direction == 3:
            direction = 2

        self.image = ImageCache['RockyBossWrench%d%d' % (big, direction)]
        if big:
            self.offset = (
                    (2, -33),
                    (-5, -33),
                    (-11, 5)
                )[direction]
        else:
            self.offset = (
                    (6, -26),
                    (-1, -26),
                    (-5, 11)
                )[direction]

        super().dataChanged()

class SpriteImage_SamuraiGuy(SLib.SpriteImage_Static):  # 19
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['SamuraiGuy'],
            (-8, -13),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('SamuraiGuy', 'samurai_guy.png')

class SpriteImage_Podouble(SLib.SpriteImage):  # 324
    def __init__(self, parent):
        super().__init__(parent)
        self.aux.append(SLib.AuxiliaryImage(parent, 110, 110))
        self.aux.append(SLib.AuxiliaryTrackObject(parent, 16, 16, SLib.AuxiliaryTrackObject.Vertical))
        self.aux[0].setPos(-45, -67)
        self.aux[1].setPos(0, -288)

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('PodoubleFire', 'podouble_fire.png')
        SLib.loadIfNotInImageCache('PodoubleIce', 'podouble_ice.png')

    def dataChanged(self):
        fire = (self.parent.spritedata[2] >> 4) & 1
        distance = self.parent.spritedata[5] & 0xFF

        self.aux[0].image = ImageCache['PodoubleFire' if fire else 'PodoubleIce']
        self.aux[1].setSize(16, 208 + (distance * 8))

        super().dataChanged()

class SpriteImage_TopmanBoss(SLib.SpriteImage_Static):  # 251
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['TopmanBoss'],
            (-47.3, -66.6)
            #(-71, -100),
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('TopmanBoss', 'topman_boss.png')

class SpriteImage_CaptainBowser(SLib.SpriteImage_Static):  # 213
    def __init__(self, parent):
        super().__init__(
            parent,
            1.5,
            ImageCache['CaptainBowser'],
        )

    @staticmethod
    def loadImages():
        SLib.loadIfNotInImageCache('CaptainBowser', 'captain_bowser.png')

class SpriteImage_MusicBlock(SLib.SpriteImage_StaticMultiple): # 17
    def __init__(self, parent):
        super().__init__(parent, 1.5)
        self.offset = (0, -45)

    @staticmethod
    def loadImages():
        if 'MusicBlock1' in ImageCache: return
        for i in range(8):
            ImageCache['MusicBlock%d' % (i + 1)] = SLib.GetImg('music_block_%d.png' % (i + 1))

    def dataChanged(self):
        colour = (self.parent.spritedata[5] & 0xF) % 9

        if colour == 0:
            self.image = ImageCache['MusicBlock1']
        else:
            self.image = ImageCache['MusicBlock%d' % colour]

        super().dataChanged()

ImageClasses = {
    13: SpriteImage_ClownCar,
    20: SpriteImage_NewerGoomba,
    21: SpriteImage_NewerParaGoomba,
    22: SpriteImage_NewerBabyGoomba,
    24: SpriteImage_NewerBuzzyBeetle,
    25: SpriteImage_NewerSpiny,
    26: SpriteImage_NewerUpsideDownSpiny,
    40: SpriteImage_NewerQSwitch,
    42: SpriteImage_NewerExcSwitch,
    47: SpriteImage_Thwomp,
    48: SpriteImage_GiantThwomp,
    57: SpriteImage_NewerKoopa,
    58: SpriteImage_NewerParaKoopa,
    60: SpriteImage_NewerSpikeTop,
    78: SpriteImage_NewerBouncyCloud,
    63: SpriteImage_NewerSpikeBall,
    98: SpriteImage_GiantSpikeBall,
    101: SpriteImage_NewerBobomb,
    107: SLib.SpriteImage,
    145: SpriteImage_NewerFloatingBarrel,
    153: SpriteImage_NewerQSwitchUnused,
    188: SpriteImage_MidwayFlag,
    191: SpriteImage_TileEventNewer,
    195: SpriteImage_NewerHuckitCrab,
    198: SpriteImage_NewerGiantGoomba,
    199: SpriteImage_NewerMegaGoomba,
    207: SpriteImage_QBlock,
    208: SpriteImage_QBlockUnused,
    209: SpriteImage_BrickBlock,
    223: SpriteImage_NewerSpringBlock,
    230: SpriteImage_NewerBramball,
    231: SpriteImage_NewerWiggleShroom,
    255: SpriteImage_RotatingQBlock,
    256: SpriteImage_RotatingBrickBlock,
    269: SpriteImage_NewerParabomb,
    296: SpriteImage_NewerMegaBuzzy,
    341: SpriteImage_NewerBigShell,
    391: SpriteImage_NewerGlowBlock,
    402: SpriteImage_LineQBlock,
    403: SpriteImage_LineBrickBlock,
    478: SpriteImage_NewerBowserSwitchSm,
    479: SpriteImage_NewerBowserSwitchLg,
    483: SpriteImage_Flipblock,
    484: SpriteImage_DragonCoasterPiece,
    485: SpriteImage_FakeStarCoin,
    486: SpriteImage_ShyGuy,
    487: SpriteImage_ShyGuyGiant,
    488: SpriteImage_StarCollectable,
    489: SpriteImage_PumpkinGoomba,
    490: SpriteImage_EventBlock,
    491: SpriteImage_ActorSpawner,
    492: SpriteImage_ActorMultiSpawner,
    493: SpriteImage_ClownCar,
    494: SpriteImage_Thundercloud,
    496: SpriteImage_MessageBlock,
    498: SpriteImage_Meteor,
    499: SpriteImage_Topman,
    500: SpriteImage_ElectricLine,
    501: SpriteImage_AngrySun,
    502: SpriteImage_FallingChestnut,
    506: SpriteImage_GigaGoomba,
    515: SpriteImage_FuzzyBear,
    516: SpriteImage_MegaThwomp,
    517: SpriteImage_Boolossus,
    518: SpriteImage_RockyBoss,
    519: SpriteImage_RockyBossWrench,
    520: SpriteImage_SamuraiGuy,
    521: SpriteImage_Podouble,
    522: SpriteImage_TopmanBoss,
    523: SpriteImage_CaptainBowser,
    529: SpriteImage_MusicBlock,
    244: SpriteImage_LineEvent,
}

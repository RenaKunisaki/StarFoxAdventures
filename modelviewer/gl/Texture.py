import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
from PIL import Image
import numpy as np
from .Sampler import Sampler


class Texture:
    """GL texture object."""

    # _formats[i] for image with (i-1) channels
    _formats = (gl.GL_RED, gl.GL_RG, gl.GL_RGB, gl.GL_RGBA)

    def __init__(self, ctx, path=None, width=None, height=None, format=None,
    textureUnit=0, sampler=True, multiSample=False):
        """Create a texture.

        path: Path to image file.
        width, height: Size of texture, in pixels.
        format: Texture format.
        textureUnit: Texture unit index to bind to.
        sampler: Either `True` to create a new Sampler, `None` to not use
            a Sampler, or an existing Sampler to use.
        multiSample: Whether to use multi-sampling.

        `width`, `height` and `format` should only be specified
        if `path` is not, and vice-versa.
        """
        self.ctx = ctx
        self.id  = None
        self.textureUnit = textureUnit

        # create a texture object
        # XXX multiSample probably needs more than just this.
        self.target = gl.GL_TEXTURE_2D_MULTISAMPLE if multiSample else gl.GL_TEXTURE_2D
        self.ctx.glActiveTexture(gl.GL_TEXTURE0 + self.textureUnit)
        self.id = self.ctx.glGenTextures(1)
        self.ctx.glBindTexture(self.target, self.id)

        # get the data and params
        if path is not None:
            pixels = self._loadImage(path)
        else:
            # XXX this needs more work.
            # also can we use these params + path to extract a
            # region/channel(s) of an image?
            if width is None or height is None or format is None:
                raise ValueError("Either `path` or (`width`, `height`, `format`) must be specified")
            self.width    = width
            self.height   = height
            self.channels = format # XXX
            pixels        = None

        # validate params
        if (self.width < 1 or self.height < 1 or self.channels < 1
        or self.channels > 4):
            raise ValueError("Invalid dimensions (%dx%dx%d)" % (
                self.width, self.height, self.channels))

        # create the pixel store
        alignment = self.channels # * bytes per channel
        self.ctx.glPixelStorei(gl.GL_PACK_ALIGNMENT,   alignment)
        self.ctx.glPixelStorei(gl.GL_UNPACK_ALIGNMENT, alignment)

        # create the image
        baseFormat = self.format
        pixelType  = gl.GL_UNSIGNED_BYTE
        self.ctx.glTexImage2D(self.target, 0, self.format, self.width,
            self.height, 0, baseFormat, pixelType, pixels)

        # set up additional params
        self.minFilter = gl.GL_LINEAR
        self.magFilter = gl.GL_LINEAR

        # create/attach the sampler
        if sampler == True:
            self._sampler = self.ctx.Sampler()
            self.bindSampler(self._sampler)
        else:
            self._sampler = sampler
            if sampler is not None: self.bindSampler(self._sampler)


    def __enter__(self):
        self.bind()
        return self

    def __exit__(self, type, value, traceback):
        self.unbind()


    def bind(self):
        self.ctx.glActiveTexture(gl.GL_TEXTURE0 + self.textureUnit)
        self.ctx.glBindTexture(self.target, self.id)

    def unbind(self):
        self.ctx.glBindTexture(self.target, 0)


    def bindSampler(self, sampler):
        self.ctx.glBindSampler(self.id, sampler.id)
        self._sampler = sampler

    def unbindSampler(self):
        self.ctx.glBindSampler(self.id, 0)
        self._sampler = None


    def _loadImage(self, path):
        image  = Image.open(path).convert('RGBA')
        pixels = image.load()
        self.width, self.height = image.size
        self.channels = len(pixels[0, 0])
        log.debug("Texture: %dx%dx%d", self.width, self.height, self.channels)
        self.format = self._formats[self.channels - 1]
        return np.array(image.getdata(), dtype=np.uint8)


    @property
    def minFilter(self):
        return self.ctx.glGetTexParameteriv(self.target, gl.GL_TEXTURE_MIN_FILTER)

    @minFilter.setter
    def minFilter(self, mode):
        self.ctx.glTexParameteri(self.target, gl.GL_TEXTURE_MIN_FILTER, mode)


    @property
    def magFilter(self):
        return self.ctx.glGetTexParameteriv(self.target, gl.GL_TEXTURE_MAG_FILTER)

    @magFilter.setter
    def magFilter(self, mode):
        self.ctx.glTexParameteri(self.target, gl.GL_TEXTURE_MAG_FILTER, mode)

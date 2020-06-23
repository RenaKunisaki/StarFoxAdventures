import gl
from importlib import resources
from . import shaders
from .. import Common

class SfaProgram(gl.Program):
    """Base for Programs used by this renderer."""
    separable = True

    def _getShaderCodeFromFile(self, path):
        # this lets us specify where to #include from.
        try: return resources.read_text(shaders, path)
        except FileNotFoundError: return resources.read_text(Common, path)


    def setMtxs(self, projection, modelView):
        # used by geometry shaders
        with self:
            self.uniforms['matProjection'].value = projection
            self.uniforms['matModelview'].value  = modelView

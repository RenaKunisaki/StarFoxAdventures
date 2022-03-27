export default class Program {
    /** A GL shader program.
     */
    constructor(context, sources=null) {
        this.ctx = context;
        this.gl  = context.gl;
        this.shaders = {};
        if(sources) this.compile(sources);
    }

    _compileShader(type, source) {
        /** Compile shader source.
         *  type: eg gl.VERTEX_SHADER
         *  source: the code to compile
         *  return: compiled shader object
         */
        const gl = this.gl;
        const shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            const msg = gl.getShaderInfoLog(shader);
            gl.deleteShader(shader);
            console.error("Failed compiling", source);
            throw new Error("Failed compiling shader: " + msg);
        }
        return shader;
    }

    compile(sources) {
        /** Compile shader program.
         *  sources: Dict of type (eg gl.VERTEX_SHADER) => code.
         */
        const gl = this.gl;
        this.program = gl.createProgram();

        for(const [type, code] of Object.entries(sources)) {
            //console.log("compile", type, code);
            const shader = this._compileShader(type, code);
            this.shaders[type] = shader;
            gl.attachShader(this.program, shader);
        }
        gl.linkProgram(this.program);
        if(!gl.getProgramParameter(this.program, gl.LINK_STATUS)) {
            throw new Error("Failed linking shader: " +
                gl.getProgramInfoLog(this.program));
        }
    }

    use() {
        this.gl.useProgram(this.program);
    }

    getAttribLocation(name) {
        return this.gl.getAttribLocation(this.program, name);
    }

    getUniformLocation(name) {
        return this.gl.getUniformLocation(this.program, name);
    }

    getLogs() {
        const result = {};
        for(const [type, shader] of Object.entries(this.shaders)) {
            result[type] = this.gl.getShaderInfoLog(shader);
        }
        return result;
    }
}

GX rendering:
- set the base address (buffer), count, format for each attribute
- set the address and format for each texture slot
- execute a display list
- repeat for each part you want to render
- a model will have several parts
    - each can have different buffer, format, textures
    - those are selected by the bitstream
    - probably the buffers don't change, but textures/formats do

DAE:
- create some elements defining each buffer
    - each has a format; changing format requires multiple copies
        - or just don't change format; read whatever but convert to a common format
    - each also needs an accessor to define stride, etc
    - textures probably work this way too?
- create a geometry element (represents the whole model)
    - create a mesh element (represents one part)
        - add a vertices element
            - for each attribute, add an input element referencing the buffer

these are probably similar enough to adapt
the bitstream only has 4 operations:
    - select texture (can change how stream is interpreted)
    - call dlist
    - set vertex format
    - set matrix (does nothing for maps)
so we can probably split off a new mesh for each operation assuming we've drawn at least one polygon since the previous

possible complication: with GX you don't always need to use buffers.
attribute values can be directly in the display list.
so we would need to ensure we can append to the buffers as we parse the lists, and only then write them to the DAE

it might be easier to only do that. add each value to the buffer as the dlist parser encounters it. only issue is we'd end up with duplicate values, but maybe we can follow it with a folding step to collapse them... though that might be difficult as it means going back through all vertices that refer to a given index and changing them to point to a different index.

or the buffers just have an "add" method that checks if they already contain that value, adds it if not, and returns an index.

so make a DaeBuffer class which manages the various attribute buffers
I think each vertex can only have one index that's used for every buffer
so we would need to add all the vertex' attributes at once
the class checks if there's already such an entry

I have no idea why the game does use inline values for some attributes and buffers for others...
the model file formats might not allow for buffers for some?
need to verify how it works for cases like TEX0, TEX1...
does each have its own index? or does TEX1 use the index of TEX0+1 or such?
looks like each has its own index (and format, so one could be inline while another is indexed)
that means there only needs to be one texcoord buffer. you'd just use different indexes for TEX0 and TEX1 (or sometimes maybe even the same)
and we know the buffers themselves don't change
in theory the same buffer offset can be accessed as several formats but I very much doubt that happens
the only time formats/sizes change probably is the "set vtx fmt" command changing between I8/I16/direct (and that means direct is always the same format even though it doesn't necessarily have to be)
and there is no way for that command to specify Direct or None, only I8 and I16. so it really only changes how dlists are parsed.
(we could get away with only ever using I16 when importing and later worry about optimizing)
- the SetVtxFmt command *can* set a format to None. in fact it sets them all to None, then reads I8 or I16 from the stream - but if a shader doesn't have a given field then it won't read them, so they remain at None.

what all that means is:
- we can just assume the buffers are always in a given format
- the attributes that have buffers never have inline values, so we don't need to worry about appending to the buffers
    - unless the dlist commands themselves change the format (can they?) but we're assuming that doesn't happen
    - could prove this by decoding every model and checking for it, but that may be difficult because some use alt render ops or maybe shader flags we haven't found yet
    - dlists can write to CP, XF, INDX A/B/C/D, BP
        - CP controls the formats so technically they can change it
- do any attributes actually use inline format?
    - looks like by default all are set to either I16 or None, except Normals (set to Direct but count=0)

- select texture (can change how stream is interpreted)
    - not sure how this works but ignore it for now
- call dlist
- set vertex format
    - we don't need to care about it; only affects how dlists are parsed
- set matrix
    - for maps the matrices aren't used

so the only command we actually need to handle at the moment is call dlist
dlist commands are mainly:
- draw
- set CP reg
    - set vtx formats, base, stride
        - none of which should be in a model
- set XF reg
    - set matrices
        - there's already a stream command for that
    - set light params
    - set various things like perf, ambient color, HW stuff
        - things that wouldn't make sense to include in a model
- set INDX reg
    - not even sure what these do
    - we ignore them and it's been fine, so
- set BP reg
    - again we don't emulate these anyway

so drawing commands are probably the only ones we need to care about.
right now the dlist parser just does RenderBatch.addVertices(mode, ...vtxs)
so we could allow for a hook to get the vertices from there
or we could iterate the resulting batch and ignore functions (feels fragile though and recursion could be an issue)
in fact it looks like the only method DlistParser calls on RenderBatch is addVertices
so, we could get away with just parsing it

no we can't because it doesn't preserve the original idxs

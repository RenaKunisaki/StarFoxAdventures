802b1a6c is where it reads most (but not all) inputs for player
that's called from playerObjUpdate so it should be called once per object.
if we changed the hardcoded controller IDs here to a field from the object, we should be able to control each one independently.

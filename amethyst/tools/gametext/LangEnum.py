import enum

class LangEnum(enum.IntEnum):
    # same IDs as game
    English  = 0,
    French   = 1,
    German   = 2,
    Italian  = 3,
    Japanese = 4,
    Spanish  = 5,

LangCodes = {
    LangEnum.English:  'en',
    LangEnum.French:   'fr',
    LangEnum.German:   'de',
    LangEnum.Italian:  'it',
    LangEnum.Japanese: 'jp',
    LangEnum.Spanish:  'es',
}

export const Language = {
    English:  0,
    French:   1,
    German:   2,
    Italian:  3,
    Japanese: 4,
    Spanish:  5,
};
export const LangById = {};
for(let [name,id] of Object.entries(Language)) LangById[id] = name;

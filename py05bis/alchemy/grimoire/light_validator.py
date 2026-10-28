from .light_spellbook import light_spell_allowed_ingredients


def validate_ingredients(ingredients: str) -> str:
    for ingredient in light_spell_allowed_ingredients():
        if ingredient in ingredients.lower():
            return f"{ingredients} - VALID"
    return f"{ingredients} - INVALID"

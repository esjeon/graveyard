#!/usr/bin/env python

import json
import os.path
from typing import List, Tuple, Dict

import lupa

FACTORIO_PATH = '/srv/file/steam/steamapps/common/Factorio/'
DIFFICULTY = 'normal' # or 'expensive'
PRETTY = False

class Items:
    def __init__(self, items: List[Tuple[str, float]] = None) -> None:
        self.items = {} # type: Dict[str, float]
        if items is not None:
            for name, amount in items:
                self.append(name, amount)

    def append(self, name: str, amount: float) -> None:
        if name in self.items:
            self.items[name] += amount
        else:
            self.items[name] = amount

    def append_lua(self, table) -> None:
        if table.probability is not None:
            self.append(str(table.name), float(table.amount) * float(table.probability))
        elif table.name is not None:
            self.append(str(table.name), float(table.amount))
        else:
            self.append(*table.values())

    def __str__(self) -> str:
        return "Items([{}])".format(", ".join(
            '("{}",{})'.format(name, amount)
            for name, amount in self.items.items()
        ))

    def __repr__(self) -> str:
        return str(self)

class Recipe:
    @staticmethod
    def from_lua(table) -> "Recipe":
        table = dict(table)
        if DIFFICULTY in table:
            table.update(dict(table[DIFFICULTY]))

        name = str(table['name']).upper()

        ingredients = Items()
        for item in list(table['ingredients'].values()):
            ingredients.append_lua(item)

        if 'result' in table:
            results = Items([(
                str(table['result']),
                float(table['result_count']) if 'result_count' in table else 1
            )])
        elif 'results' in table:
            results = Items()
            for item in list(table['results'].values()):
                results.append_lua(item)

        duration = float(table['energy_required']) if 'energy_required' in table else 0.5

        return Recipe(name, ingredients, results, duration)

    def __init__(self, name: str, ingredients: Items, results: Items, duration: float) -> None:
        self.name = name
        self.ingredients = ingredients
        self.results = results
        self.duration = duration

    def __str__(self) -> str:
        return 'Recipe("{}", {}, {}, {})'.format(self.name, self.ingredients, self.results, self.duration)

    def __repr__(self) -> str:
        return str(self)


class RecipeJSONEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Recipe):
            return {
                'name': obj.name,
                'ingredients': obj.ingredients,
                'results': obj.results,
                'duration': obj.duration
            }
        elif isinstance(obj, Items):
            return obj.items
        return json.JSONEncoder.default(self, obj)


def main():
    lua = lupa.LuaRuntime(unpack_returned_tuples=True)

    lua.globals().package.path += ';' + os.path.join(FACTORIO_PATH, 'data', 'core', 'lualib', '?.lua')
    lua.globals().package.path += ';' + os.path.join(FACTORIO_PATH, 'data', 'base', '?.lua')

    lua.require('dataloader')

    for filename in os.listdir(os.path.join(FACTORIO_PATH, 'data', 'base', 'prototypes', 'recipe')):
        lua.require('prototypes.recipe.' + filename.replace('.lua', ''))

    recipes = [
        Recipe.from_lua(table)
        for name, table in lua.globals().data.raw.recipe.items()
    ]

    output = json.dumps(recipes, cls=RecipeJSONEncoder, indent=4, sort_keys=True)
    with open('recipe.json', 'w') as output_file:
        output_file.write(output)

    return recipes

if __name__ == "__main__":
    RECIPES = main()

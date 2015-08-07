
function parse_very_simple(input) {
  return eval((' ' + input + ' ')
    .replace(/\(/g,' [ ')
    .replace(/\)/g,' ] ')
    .split(/\s+/)
    .slice(1, -1) // remove empty strings
    .join('","')
    .replace(/"?\[(",)?/g, '[')
    .replace(/(,")?\]"?/g, ']')
  )
}

console.log(
parse_very_simple("( have 1 2 (fun (x y) (+ x y)))")
)


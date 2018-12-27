macro_rules! elem {
    ($tag:ident, {$($attr:tt)*}) => {
        println!("tag='{}'", stringify!(tag));
        attrs!($($attr)*);
    }
}

macro_rules! attrs {
    ($($key:ident : $val:expr),*) => {{
        $(show(stringify!($key), $val);)*
    }};
}

fn show(a: &str, b: &str) {
    println!("{}='{}'", a, b);
}

fn main() {
    elem!(div, {
        style: "width:100px",
        class: "C1 C2",
        id: "asdf"
    });
}


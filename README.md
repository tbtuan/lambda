# lambda

A simple interpreter

## Grammar

```
<Expression> -> <Identifier>
             -> <Integer>
             -> ( <Expression> <Expression> )
             -> ( lambda <Expression> <Expression> )
             -> ( define <Expression> <Expression> )
             -> ( if <Expression> <Expression> <Expression> )
```

template<class T> class ExpressionTemplate;
typedef ExpressionTemplate<void> Expression;
template<class T> class ExpressionTemplate : public ReferenceCounted
{
public:
    static Symbol parse(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence14(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            static String logicalAnd("||");
            if (Space::parseOperator(source, logicalAnd)) {
                Symbol e2 = parsePrecedence14(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomLogicalOr, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    virtual void compile() = 0;
    virtual Symbol type() const = 0;
    virtual void push(Stack<Value>* stack) = 0;
    virtual bool isLValue() = 0;
    virtual Variable* variable(Stack<Value>* stack) = 0;
private:
    static Symbol parsePrecedence14(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence13(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            static String logicalAnd("&&");
            if (Space::parseOperator(source, logicalAnd)) {
                Symbol e2 = parsePrecedence13(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomLogicalAnd, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    static Symbol parsePrecedence13(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence12(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            if (Space::parseCharacter(source, '|')) {
                Symbol e2 = parsePrecedence12(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomBitwiseOr, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    static Symbol parsePrecedence12(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence11(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            if (Space::parseCharacter(source, '~')) {
                Symbol e2 = parsePrecedence11(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomBitwiseXor, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    static Symbol parsePrecedence11(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence10(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            if (Space::parseCharacter(source, '&')) {
                Symbol e2 = parsePrecedence10(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomBitwiseAnd, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    static Symbol parsePrecedence10(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence9(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            static String equalTo("==");
            if (Space::parseOperator(source, equalTo)) {
                Symbol e2 = parsePrecedence9(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomEqualTo, symbolFromLocation(location), e, e2);
                continue;
            }
            static String notEqualTo("!=");
            if (Space::parseOperator(source, notEqualTo)) {
                Symbol e2 = parsePrecedence9(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomNotEqualTo, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    static Symbol parsePrecedence9(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence8(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            static String lessThanOrEqualTo("<=");
            if (Space::parseOperator(source, lessThanOrEqualTo)) {
                Symbol e2 = parsePrecedence8(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomLessThanOrEqualTo, symbolFromLocation(location), e, e2);
                continue;
            }
            static String greaterThanOrEqualTo(">=");
            if (Space::parseOperator(source, greaterThanOrEqualTo)) {
                Symbol e2 = parsePrecedence8(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomGreaterThanOrEqualTo, symbolFromLocation(location), e, e2);
                continue;
            }
            if (Space::parseCharacter(source, '<')) {
                Symbol e2 = parsePrecedence8(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomLessThan, symbolFromLocation(location), e, e2);
                continue;
            }
            if (Space::parseCharacter(source, '>')) {
                Symbol e2 = parsePrecedence8(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomGreaterThan, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }

    static Symbol parsePrecedence8(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence7(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            static String leftShift("<<");
            if (Space::parseOperator(source, leftShift)) {
                Symbol e2 = parsePrecedence7(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomLeftShift, symbolFromLocation(location), e, e2);
                continue;
            }
            static String rightShift(">>");
            if (Space::parseOperator(source, rightShift)) {
                Symbol e2 = parsePrecedence7(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomRightShift, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }
    static Symbol parsePrecedence7(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence6(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            if (Space::parseCharacter(source, '+')) {
                Symbol e2 = parsePrecedence6(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomAdd, symbolFromLocation(location), e, e2);
                continue;
            }
            if (Space::parseCharacter(source, '-')) {
                Symbol e2 = parsePrecedence6(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomSubtract, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }
    static Symbol parsePrecedence6(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence4(source, scope);
        if (!e.valid())
            return 0;
        do {
            DiagnosticLocation location = source->location();
            if (Space::parseCharacter(source, '*')) {
                Symbol e2 = parsePrecedence4(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomMultiply, symbolFromLocation(location), e, e2);
                continue;
            }
            if (Space::parseCharacter(source, '/')) {
                Symbol e2 = parsePrecedence4(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomDivide, symbolFromLocation(location), e, e2);
                continue;
            }
            if (Space::parseCharacter(source, '%')) {
                Symbol e2 = parsePrecedence4(source, scope);
                if (!e2.valid())
                    throwError(source);
                e = TupleSymbol(atomModulo, symbolFromLocation(location), e, e2);
                continue;
            }
            return e;
        } while (true);
    }
    static Symbol parsePrecedence4(CharacterSource* source, Scope* scope)
    {
        DiagnosticLocation location = source->location();
        if (Space::parseCharacter(source, '!')) {
            Symbol e = parsePrecedence4(source, scope);
            return TupleSymbol(atomLogicalNot, symbolFromLocation(location), e);
        }
        if (Space::parseCharacter(source, '~')) {
            Symbol e = parsePrecedence4(source, scope);
            return TupleSymbol(atomBitwiseNot, symbolFromLocation(location), e);
        }
        if (Space::parseCharacter(source, '+')) {
            Symbol e = parsePrecedence4(source, scope);
            return TupleSymbol(atomPositive, symbolFromLocation(location), e);
        }
        if (Space::parseCharacter(source, '-')) {
            Symbol e = parsePrecedence4(source, scope);
            return TupleSymbol(atomNegative, symbolFromLocation(location), e);
        }
        if (Space::parseCharacter(source, '*')) {
            Symbol e = parsePrecedence4(source, scope);
            return TupleSymbol(atomDereference, symbolFromLocation(location), e);
        }
        if (Space::parseCharacter(source, '&')) {
            Symbol e = parsePrecedence4(source, scope);
            return TupleSymbol(atomAddressOf, symbolFromLocation(location), e);
        }
        return parsePrecedence3(source, scope);
    }
    static Symbol parsePrecedence3(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence2(source, scope);
        if (!e.valid())
            return 0;
        DiagnosticLocation location = source->location();
        if (Space::parseCharacter(source, '^')) {
            Symbol e2 = parsePrecedence3(source, scope);
            if (!e2.valid())
                throwError(source);
                e = TupleSymbol(atomPower, symbolFromLocation(location), e, e2);
        }
        return e;
    }
    static Symbol parsePrecedence2(CharacterSource* source, Scope* scope)
    {
        Symbol e = parsePrecedence0(source, scope);
        if (!e.valid())
            return 0;
        do {
            if (!Space::parseCharacter(source, '('))
                break;
            e = FunctionCallExpression::parse(source, scope, e);
            if (!e.valid())
                throwError(source);
        } while (true);
        return e;
    }
    static Symbol parsePrecedence0(CharacterSource* source, Scope* scope)
    {
        Symbol e = DoubleQuotedString::parse(source, scope);
        if (e.valid())
            return e;
        e = EmbeddedLiteral::parse(source);
        if (e.valid())
            return e;
        e = Integer::parse(source);
        if (e.valid())
            return e;
        e = Identifier::parse(source, scope);
        if (e.valid())
            return e;
        if (Space::parseCharacter(source, '(')) {
            e = parse(source, scope);
            Space::assertCharacter(source, ')');
            return e;
        }
        e = BooleanConstant::parse(source);
        if (e.valid())
            return e;
        e = NullConstant::parse(source);
        if (e.valid())
            return e;
        return 0;
    }
    static void throwError(CharacterSource* source)
    {
        static String expected("Expected expression");
        source->location().throwError(expected);
    }
};

class Identifier : public Expression
{
public:
    static Reference<Identifier> parse(CharacterSource* source, Scope* scope)
    {
        CharacterSource s = *source;
        DiagnosticLocation location = s.location();
        int start = s.offset();
        int c = s.get();
        if (c < 'a' || c > 'z')
            return 0;
        CharacterSource s2;
        do {
            s2 = s;
            c = s.get();
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
                continue;
            break;
        } while (true);
        int end = s2.offset();
        Space::parse(&s2);
        String name = s2.subString(start, end);
        static String keywords[] = {
            String("assembly"),
            String("break"),
            String("case"),
            String("catch"),
            String("continue"),
            String("default"),
            String("delete"),
            String("do"),
            String("done"),
            String("else"),
            String("elseIf"),
            String("elseUnless"),
            String("false"),
            String("finally"),
            String("from"),
            String("for"),
            String("forever"),
            String("if"),
            String("in"),
            String("new"),
            String("nothing"),
            String("null"),
            String("return"),
            String("switch"),
            String("this"),
            String("throw"),
            String("true"),
            String("try"),
            String("unless"),
            String("until"),
            String("while")
        };
        for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); ++i)
            if (name == keywords[i])
                return 0;
        *source = s2;
        return new Identifier(scope, name, location);
    }
    Symbol type() const { return _symbolName->type(_name, _location); }
    String name() const { return _name; }
    void push(Stack<Value>* stack)
    {
        stack->push(_symbolName->value());
    }
    void compile()
    {
        _symbolName = _scope->resolveSymbolName(_name, _location);
    }
    bool isLValue()
    {
        Reference<Variable> variable = _symbolName;
        return variable.valid();
    }
    Variable* variable(Stack<Value>* stack)
    {
        Reference<Variable> variable = _symbolName;
        return variable;
    }
private:
    Identifier(Scope* scope, String name, DiagnosticLocation location)
      : _scope(scope), _name(name), _location(location)
    { }
    Scope* _scope;
    String _name;
    Reference<SymbolName> _symbolName;
    DiagnosticLocation _location;
};

class RValueExpression : public Expression
{
public:
    bool isLValue() { return false; }
    Variable* variable(Stack<Value>* stack) { return 0; }
};

class Integer : public RValueExpression
{
public:
    static Reference<Integer> parse(CharacterSource* source)
    {
        CharacterSource s = *source;
        int n = 0;
        int c = s.get();
        if (c < '0' || c > '9')
            return 0;
        do {
            n = n*10 + c - '0';
            *source = s;
            c = s.get();
            if (c < '0' || c > '9') {
                Space::parse(source);
                return new Integer(n);
            }
        } while (true);
    }
    void compile() { }
    Symbol type() const { return Symbol(atomInt); }
    void push(Stack<Value>* stack)
    {
        stack->push(Value(_n));
    }
private:
    Integer(int n) : _n(n) { }
    int _n;
};

class BooleanConstant : public RValueExpression
{
public:
    static Reference<BooleanConstant> parse(CharacterSource* source)
    {
        static String trueKeyword("true");
        if (Space::parseKeyword(source, trueKeyword))
            return new BooleanConstant(true);
        static String falseKeyword("false");
        if (Space::parseKeyword(source, falseKeyword))
            return new BooleanConstant(false);
        return 0;
    }
    void compile() { }
    Symbol type() const { return Symbol(atomBoolean); }
    void push(Stack<Value>* stack)
    {
        stack->push(Value(_value ? 1 : 0));
    }
private:
    BooleanConstant(bool value) : _value(value) { }
    bool _value;
};

class NullConstant : public RValueExpression
{
public:
    static Reference<NullConstant> parse(CharacterSource* source)
    {
        static String keyword("null");
        if (Space::parseKeyword(source, keyword))
            return new NullConstant();
        return 0;
    }
    void compile() { }
    Symbol type() const { return Symbol(atomPointer, Symbol(atomVoid)); }
    void push(Stack<Value>* stack)
    {
        stack->push(Value(0));
    }
};

class LogicalAndExpression : public RValueExpression
{
public:
    LogicalAndExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Symbol type() const
    {
        if (_left->type() != BooleanType() || _right->type() != BooleanType()) {
           static String error("Both operands of && must be of Boolean type.");
           _location.throwError(error);
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _left->push(stack);
        Value l = stack->pop();
        if (l.getInt() == 0)
            stack->push(Value(0));
        _right->push(stack);
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class LogicalOrExpression : public RValueExpression
{
public:
    LogicalOrExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        if (_left->type() != BooleanType() || _right->type() != BooleanType()) {
           static String error("Both operands of || must be of Boolean type.");
           _location.throwError(error);
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _left->push(stack);
        Value l = stack->pop();
        if (l.getInt() != 0)
            stack->push(Value(1));
        _right->push(stack);
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class BitwiseAndExpression : public RValueExpression
{
public:
    BitwiseAndExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == BooleanType() && r == BooleanType()))) {
           static String error1("Don't know how to AND a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return l;
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() & r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class BitwiseOrExpression : public RValueExpression
{
public:
    BitwiseOrExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == BooleanType() && r == BooleanType()))) {
           static String error1("Don't know how to OR a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return l;
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() | r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class BitwiseXorExpression : public RValueExpression
{
public:
    BitwiseXorExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == BooleanType() && r == BooleanType()))) {
           static String error1("Don't know how to XOR a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return l;
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() ^ r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class EqualToExpression : public RValueExpression
{
public:
    EqualToExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == StringType() && r == StringType()) || (l == BooleanType() && r == BooleanType()))) {
           static String error1("Don't know how to compare a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        if (_left->type() == StringType())
            stack->push(Value(l.getString() == r.getString() ? 1 : 0));
        else
            stack->push(Value(l.getInt() == r.getInt() ? 1 : 0));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class NotEqualToExpression : public RValueExpression
{
public:
    NotEqualToExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == StringType() && r == StringType()) || (l == BooleanType() && r == BooleanType()))) {
           static String error1("Don't know how to compare a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        if (_left->type() == StringType())
            stack->push(Value(l.getString() != r.getString() ? 1 : 0));
        else
            stack->push(Value(l.getInt() != r.getInt() ? 1 : 0));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class LessThanExpression : public RValueExpression
{
public:
    LessThanExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == StringType() && r == StringType()))) {
           static String error1("Don't know how to compare a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        if (_left->type() == IntType())
            stack->push(Value(l.getInt() < r.getInt() ? 1 : 0));
        else
            stack->push(Value(l.getString() < r.getString() ? 1 : 0));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class GreaterThanExpression : public RValueExpression
{
public:
    GreaterThanExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == StringType() && r == StringType()))) {
           static String error1("Don't know how to compare a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        if (_left->type() == IntType())
            stack->push(Value(l.getInt() > r.getInt() ? 1 : 0));
        else
            stack->push(Value(l.getString() > r.getString() ? 1 : 0));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class LessThanOrEqualToExpression : public RValueExpression
{
public:
    LessThanOrEqualToExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == StringType() && r == StringType()))) {
           static String error1("Don't know how to compare a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        if (_left->type() == IntType())
            stack->push(Value(l.getInt() <= r.getInt() ? 1 : 0));
        else
            stack->push(Value(l.getString() <= r.getString() ? 1 : 0));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class GreaterThanOrEqualToExpression : public RValueExpression
{
public:
    GreaterThanOrEqualToExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (!((l == IntType() && r == IntType()) || (l == StringType() && r == StringType()))) {
           static String error1("Don't know how to compare a ");
           static String error2(" with a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        if (_left->type() == IntType())
            stack->push(Value(l.getInt() >= r.getInt() ? 1 : 0));
        else
            stack->push(Value(l.getString() >= r.getString() ? 1 : 0));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class ShiftLeftExpression : public RValueExpression
{
public:
    ShiftLeftExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to shift a ");
           static String error2(" left by a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() << r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class ShiftRightExpression : public RValueExpression
{
public:
    ShiftRightExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to shift a ");
           static String error2(" right by a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() >> r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class AddExpression : public RValueExpression
{
public:
    AddExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l == StringType())
            if (r == IntType() || r == StringType() || r == BooleanType())
                return StringType();
        if (r == StringType())
            if (l == IntType() || r == BooleanType())
                return StringType();
        if (l != IntType() || r != IntType()) {
            static String error1("Don't know how to add a ");
            static String error2(" to a ");
            _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        Type left = _left->type();
        Type right = _right->type();
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        static String trueString("true");
        static String falseString("false");
        if (left == StringType())
            if (right == StringType())
                stack->push(Value(l.getString() + r.getString()));
            else
                if (right == IntType())
                    stack->push(Value(l.getString() + String::decimal(r.getInt())));
                else
                    stack->push(Value(l.getString() + (r.getInt() != 0 ? trueString : falseString)));
        else
            if (right == StringType())
                stack->push(Value(String::decimal(l.getInt()) + r.getString()));
            else
                if (right == IntType())
                    stack->push(Value(l.getInt() + r.getInt()));
                else
                    stack->push(Value((l.getInt() != 0 ? trueString : falseString) + r.getString()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class SubtractExpression : public RValueExpression
{
public:
    SubtractExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to subtract a ");
           static String error2(" from a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() - r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class MultiplyExpression : public RValueExpression
{
public:
    MultiplyExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to multiply a ");
           static String error2(" by a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() * r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class DivideExpression : public RValueExpression
{
public:
    DivideExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to divide a ");
           static String error2(" by a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() / r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class ModuloExpression : public RValueExpression
{
public:
    ModuloExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to find the remainder after division of a ");
           static String error2(" by a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(l.getInt() % r.getInt()));
    }
private:
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

class NegativeExpression : public RValueExpression
{
public:
    NegativeExpression(Symbol expression, DiagnosticLocation location)
      : _expression(expression), _location(location)
    { }
    Type type() const
    {
        Type type = _expression->type();
        if (type != IntType()) {
           static String error("Don't know how to negate a ");
           _location.throwError(error + type.toString());
        }
        return IntType();
    }
    void compile()
    {
        _expression->compile();
    }
    void push(Stack<Value>* stack)
    {
        _expression->push(stack);
        stack->push(Value(-stack->pop().getInt()));
    }
private:
    Symbol _expression;
    DiagnosticLocation _location;
};

class PositiveExpression : public RValueExpression
{
public:
    PositiveExpression(Symbol expression, DiagnosticLocation location)
      : _expression(expression), _location(location)
    { }
    Type type() const
    {
        Type type = _expression->type();
        if (type != IntType()) {
           static String error("Can't use unary + with a ");
           _location.throwError(error + type.toString());
        }
        return IntType();
    }
    void compile()
    {
        _expression->compile();
    }
    void push(Stack<Value>* stack) { }
private:
    Symbol _expression;
    DiagnosticLocation _location;
};

class BitwiseNotExpression : public RValueExpression
{
public:
    BitwiseNotExpression(Symbol expression, DiagnosticLocation location)
      : _expression(expression), _location(location)
    { }
    Type type() const
    {
        Type type = _expression->type();
        if (type != IntType() || type != BooleanType()) {
           static String error("Don't know how to bitwise NOT a ");
           _location.throwError(error + type.toString());
        }
        return type;
    }
    void compile()
    {
        _expression->compile();
    }
    void push(Stack<Value>* stack)
    {
        _expression->push(stack);
        stack->push(Value(~stack->pop().getInt()));
    }
private:
    Symbol _expression;
    DiagnosticLocation _location;
};

class LogicalNotExpression : public RValueExpression
{
public:
    LogicalNotExpression(Symbol expression, DiagnosticLocation location)
      : _expression(expression), _location(location)
    { }
    Type type() const
    {
        Type type = _expression->type();
        if (type != BooleanType()) {
           static String error("Don't know how to logical NOT a ");
           _location.throwError(error + type.toString());
        }
        return BooleanType();
    }
    void compile()
    {
        _expression->compile();
    }
    void push(Stack<Value>* stack)
    {
        _expression->push(stack);
        stack->push(Value(!stack->pop().getInt()));
    }
private:
    Symbol _expression;
    DiagnosticLocation _location;
};

class DereferenceExpression : public Expression
{
public:
    DereferenceExpression(Symbol expression, DiagnosticLocation location)
      : _expression(expression), _location(location)
    { }
    Type type() const
    {
        Type type = _expression->type();
        Type referentType = type.referentType();
        if (!referentType.valid()) {
           static String error("Don't know how dereference a ");
           _location.throwError(error + type.toString());
        }
        return referentType;
    }
    void compile()
    {
        _expression->compile();
    }
    void push(Stack<Value>* stack)
    {
        stack->push(variable(stack)->value());
    }
    bool isLValue() { return true; }
    Variable* variable(Stack<Value>* stack)
    {
        _expression->push(stack);
        return stack->pop().getPointer();
    }
private:
    Symbol _expression;
    DiagnosticLocation _location;
};

class AddressOfExpression : public RValueExpression
{
public:
    AddressOfExpression(Symbol expression, DiagnosticLocation location)
      : _expression(expression), _location(location)
    { }
    Type type() const
    {
        return PointerType(_expression->type());
    }
    void compile()
    {
        _expression->compile();
        if (!_expression->isLValue()) {
            static String lValueRequired("LValue required");
            _location.throwError(lValueRequired);
        }
    }
    void push(Stack<Value>* stack)
    {
        stack->push(Value(_expression->variable(stack)));
    }
private:
    Symbol _expression;
    DiagnosticLocation _location;
};

class PowerExpression : public RValueExpression
{
public:
    PowerExpression(Symbol left, Symbol right, DiagnosticLocation location)
      : _left(left), _right(right), _location(location)
    { }
    Type type() const
    {
        Type l = _left->type();
        Type r = _right->type();
        if (l != IntType() || r != IntType()) {
           static String error1("Don't know how to find the remainder after division of a ");
           static String error2(" by a ");
           _location.throwError(error1 + l.toString() + error2 + r.toString());
        }
        return IntType();
    }
    void compile()
    {
        _left->compile();
        _right->compile();
    }
    void push(Stack<Value>* stack)
    {
        _right->push(stack);
        _left->push(stack);
        Value l = stack->pop();
        Value r = stack->pop();
        stack->push(Value(power(l.getInt(), r.getInt())));
    }
private:
    int power(int a, int b)
    {
        if (b < 0)
            return 1/power(a, -b);
        int r = 1;
        while (b != 0) {
            if ((b & 1) != 0)
                r *= a;
            b >>= 1;
            a *= a;
        }
        return r;
    }
    Symbol _left;
    Symbol _right;
    DiagnosticLocation _location;
};

template<class T> class FunctionCallExpressionTemplate;
typedef FunctionCallExpressionTemplate<void> FunctionCallExpression;
template<class T> class FunctionCallExpressionTemplate : public Expression
{
public:
    static Reference<FunctionCallExpression> parse(CharacterSource* source, Scope* scope, Symbol function)
    {
        DiagnosticLocation location = source->location();
        int n = 0;
        Stack<Symbol > stack;
        if (!Space::parseCharacter(source, ')')) {
            do {
                Symbol e = Expression::parse(source, scope);
                if (!e.valid()) {
                    static String expression("Expected expression");
                    source->location().throwError(expression);
                }
                stack.push(e);
                ++n;
                if (Space::parseCharacter(source, ')'))
                    break;
                Space::assertCharacter(source, ',');
            } while (true);
        }
        Reference<FunctionCallExpression> functionCall = new FunctionCallExpression(scope, function, n, location);
        stack.toArray(&functionCall->_arguments);
        return functionCall;
    }
    Type type() const
    {
        return _function->type().returnType();
    }
    void compile()
    {
        _function->compile();
        TypeList argumentTypes;
        for (int i = 0; i < _arguments.count(); ++i) {
            _arguments[i]->compile();
            argumentTypes.push(_arguments[i]->type());
        }
        argumentTypes.finalize();
        Reference<Identifier> identifier(_function);
        if (identifier != 0)
            _functionDeclaration = _scope->resolveFunction(identifier, argumentTypes, _location);
    }
    void push(Stack<Value>* stack)
    {
        for (int i = _arguments.count() - 1; i >= 0; --i)
            _arguments[i]->push(stack);
        Identifier* identifier = dynamic_cast<Identifier*>(static_cast<Expression*>(_function));
        if (_functionDeclaration != 0)
            _functionDeclaration->call(stack);
        else {
            _function->push(stack);
            stack->pop().getFunctionDeclaration()->call(stack);
        }
    }
    bool isLValue() { return false; }
    Variable* variable(Stack<Value>* stack) { return 0; }
private:
    FunctionCallExpressionTemplate(Scope* scope, Symbol function, int n, DiagnosticLocation location)
      : _scope(scope), _function(function), _location(location)
    {
        _arguments.allocate(n);
    }

    Scope* _scope;
    Symbol _function;
    FunctionDeclarationStatement* _functionDeclaration;
    Array<Symbol > _arguments;
    DiagnosticLocation _location;
};

class DoubleQuotedString : public RValueExpression
{
public:
    static Symbol parse(CharacterSource* source, Scope* scope)
    {
        static String empty("");
        static String endOfFile("End of file in string");
        static String endOfLine("End of line in string");
        static String printableCharacter("printable character");
        static String escapedCharacter("escaped character");
        static String hexadecimalDigit("hexadecimal digit");
        static String newLine = String::codePoint(10);
        static String tab = String::codePoint(9);
        static String backslash = String::codePoint('\\');
        static String doubleQuote = String::codePoint('"');
        static String dollar = String::codePoint('$');
        static String singleQuote = String::codePoint('\'');
        static String backQuote = String::codePoint('`');
        if (!source->parse('"'))
            return 0;
        int start = source->offset();
        int end;
        String insert(empty);
        int n;
        int nn;
        String string(empty);
        Symbol expression;
        Symbol part;
        DiagnosticLocation location;
        DiagnosticLocation location2;
        do {
            CharacterSource s = *source;
            end = s.offset();
            int c = s.get();
            if (c < 0x20 && c != 10) {
                if (c == -1)
                    source->location().throwError(endOfFile);
                source->throwUnexpected(printableCharacter, String::hexadecimal(c, 2));
            }
            *source = s;
            switch (c) {
                case '"':
                    string += s.subString(start, end);
                    Space::parse(source);
                    return combine(expression, new DoubleQuotedString(string), location);
                case '\\':
                    string += s.subString(start, end);
                    c = s.get();
                    if (c < 0x20) {
                        if (c == 10)
                            source->location().throwError(endOfLine);
                        if (c == -1)
                            source->location().throwError(endOfFile);
                        source->throwUnexpected(escapedCharacter, String::hexadecimal(c, 2));
                    }
                    *source = s;
                    switch (c) {
                        case 'n':
                            insert = newLine;
                            break;
                        case 't':
                            insert = tab;
                            break;
                        case '$':
                            insert = dollar;
                            break;
                        case '"':
                            insert = doubleQuote;
                            break;
                        case '\'':
                            insert = singleQuote;
                            break;
                        case '`':
                            insert = backQuote;
                            break;
                        case 'U':
                            source->assert('+');
                            n = 0;
                            for (int i = 0; i < 4; ++i) {
                                nn = parseHexadecimalCharacter(source, scope);
                                if (nn == -1) {
                                    s = *source;
                                    source->throwUnexpected(hexadecimalDigit, String::codePoint(s.get()));
                                }
                                n = (n << 4) | nn;
                            }
                            nn = parseHexadecimalCharacter(source, scope);
                            if (nn != -1) {
                                n = (n << 4) | nn;
                                nn = parseHexadecimalCharacter(source, scope);
                                if (nn != -1)
                                    n = (n << 4) | nn;
                            }
                            insert = String::codePoint(n);
                            break;
                        default:
                            source->throwUnexpected(escapedCharacter, String::codePoint(c));
                    }
                    string += insert;
                    start = source->offset();
                    break;
                case '$':
                    location2 = source->location();
                    part = Identifier::parse(source, scope);
                    if (!part.valid()) {
                        if (Space::parseCharacter(source, '(')) {
                            part = Expression::parse(source, scope);
                            source->assert(')');
                        }
                    }
                    string += s.subString(start, end);
                    start = source->offset();
                    if (part.valid()) {
                        expression = combine(expression, new DoubleQuotedString(string), location);
                        string = empty;
                        expression = combine(expression, part, location2);
                        location = source->location();
                    }
                    break;
            }
        } while (true);
    }
    void compile() { }
    Type type() const { return StringType(); }
    void push(Stack<Value>* stack) { stack->push(Value(_string)); }
private:
    static Symbol combine(Symbol left, Symbol right, DiagnosticLocation location)
    {
        if (left.valid())
            return new AddExpression(left, right, location);
        return right;
    }
    DoubleQuotedString(String string) : _string(string) { }

    static int parseHexadecimalCharacter(CharacterSource* source,
        Scope* scope)
    {
        CharacterSource s = *source;
        int c = s.get();
        if (c >= '0' && c <= '9') {
            *source = s;
            return c - '0';
        }
        if (c >= 'A' && c <= 'F') {
            *source = s;
            return c + 10 - 'A';
        }
        if (c >= 'a' && c <= 'f') {
            *source = s;
            return c + 10 - 'a';
        }
        return -1;
    }
    String _string;
};

class EmbeddedLiteral : public RValueExpression
{
public:
    static Reference<EmbeddedLiteral> parse(CharacterSource* source)
    {
        static String empty;
        static String endOfFile("End of file in string");
        if (!source->parse('#'))
            return 0;
        if (!source->parse('#'))
            return 0;
        if (!source->parse('#'))
            return 0;
        int start = source->offset();
        CharacterSource s = *source;
        do {
            int c = s.get();
            if (c == -1)
                source->location().throwError(endOfFile);
            if (c == 10)
                break;
            *source = s;
        } while (true);
        int end = source->offset();
        String terminator = source->subString(start, end);
        start = s.offset();
        CharacterSource terminatorSource(terminator, empty);
        int cc = terminatorSource.get();
        String string;
        do {
            *source = s;
            int c = s.get();
            if (c == -1)
                source->location().throwError(endOfFile);
            if (cc == -1) {
                if (c != '#')
                    continue;
                CharacterSource s2 = s;
                if (s2.get() != '#')
                    continue;
                if (s2.get() != '#')
                    continue;
                string += s.subString(start, source->offset());
                *source = s2;
                Space::parse(source);
                return new EmbeddedLiteral(string);
            }
            else
                if (c == cc) {
                    CharacterSource s2 = s;
                    CharacterSource st = terminatorSource;
                    do {
                        int ct = st.get();
                        if (ct == -1) {
                            if (s2.get() != '#')
                                break;
                            if (s2.get() != '#')
                                break;
                            if (s2.get() != '#')
                                break;
                            string += s.subString(start, source->offset());
                            *source = s2;
                            Space::parse(source);
                            return new EmbeddedLiteral(string);
                        }
                        int cs = s2.get();
                        if (ct != cs)
                            break;
                    } while (true);
                }
            if (c == 10) {
                string += s.subString(start, source->offset()) + String::codePoint(10);
                start = s.offset();
            }
        } while (true);
    }
    void compile() { }
    Type type() const { return StringType(); }
    void push(Stack<Value>* stack) { stack->push(Value(_string)); }
private:
    EmbeddedLiteral(String string) : _string(string) { }

    String _string;
};

void checkBoolean(TupleSymbol expression)
{
    if (!
}

TupleSymbol typeFromExpression(TupleSymbol expression)
{
    TupleSymbol tuple = expression.tuple();
    switch (tuple.atom()) {
        case atomLogicalOr:
        case atomLogicalAnd:
            if (typeFromExpression(tuple.symbol1()).atom() != atomBoolean) {
                expectedBoolean(tuple.symbol1());


        case atomEqualTo:
        case atomNotEqualTo:
        case atomLessThanOrEqualTo:
        case atomGreaterThanOrEqualTo:
        case atomLessThan:
        case atomGreaterThan:

    atomLogicalAnd,
    atomBitwiseOr,
    atomBitwiseXor,
    atomBitwiseAnd,
    atomEqualTo,
    atomNotEqualTo,
    atomLessThanOrEqualTo,
    atomGreaterThanOrEqualTo,
    atomLessThan,
    atomGreaterThan,
    atomLeftShift,
    atomRightShift,
    atomAdd,
    atomSubtract,
    atomMultiply,
    atomDivide,
    atomModulo,
    atomLogicalNot,
    atomBitwiseNot,
    atomPositive,
    atomNegative,
    atomDereference,
    atomAddressOf,
    atomPower,

    }
}
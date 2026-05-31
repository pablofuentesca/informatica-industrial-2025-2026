#pragma once
class Mundo;

class Conjuro {
protected:
    bool usado{ false };
public:
    virtual ~Conjuro() = default;
    virtual bool aplicar(Mundo& m, int equipo) = 0;
    virtual const char* nombre() const = 0;
    bool estaUsado() const { return usado; }
    void marcarUsado() { usado = true; }
};

// Mueve una pieza aliada a una casilla libre al azar
class ConjuroTeleport : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Teleport"; }
};

// Cura completamente a todas las piezas aliadas
class ConjuroCurar : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Curar"; }
};

// Invierte el sentido del ciclo de luz del tablero
class ConjuroCambioCiclo : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Ciclo"; }
};

// Intercambia la posicion de dos piezas aliadas
class ConjuroIntercambiar : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Cambiar"; }
};

// Invoca un elemental aliado junto al entrenador
class ConjuroInvocar : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Elemental"; }
};

// Resucita la ultima pieza aliada eliminada junto al entrenador
class ConjuroRevivir : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Revivir"; }
};

// Inmoviliza una pieza enemiga durante 3 turnos
class ConjuroEncarcelar : public Conjuro {
public:
    bool aplicar(Mundo& m, int equipo) override;
    const char* nombre() const override { return "Encarcelar"; }
};

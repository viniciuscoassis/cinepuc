-- Cria tabela Sala
CREATE TABLE Sala (
    id INTEGER PRIMARY KEY,
    nome TEXT NOT NULL,
    capacidade INTEGER DEFAULT 30 NOT NULL
);

-- Cria tabela Filmes
CREATE TABLE Filmes (
    id INTEGER PRIMARY KEY,
    nome TEXT NOT NULL,
    duracao INTEGER NOT NULL,
    classificacao TEXT NOT NULL,
    genero TEXT NOT NULL,
    sinopse TEXT NOT NULL,
    lancamento TEXT NOT NULL
);

-- Cria tabela Sessao
CREATE TABLE Sessao (
    id INTEGER PRIMARY KEY,
    filme_id INTEGER NOT NULL,
    sala_id INTEGER NOT NULL,
    horario DATETIME NOT NULL,
    preco REAL NOT NULL,
    FOREIGN KEY(filme_id) REFERENCES Filmes(id),
    FOREIGN KEY(sala_id) REFERENCES Sala(id)
);

-- Cria tabela Cadeiras
CREATE TABLE Cadeiras (
    id INTEGER PRIMARY KEY,
    sala_id INTEGER NOT NULL,
    numero INTEGER NOT NULL,
    disponivel INTEGER DEFAULT 1,
    FOREIGN KEY(sala_id) REFERENCES Sala(id)
);

-- Cria tabela Usuario
CREATE TABLE Usuario (
    id INTEGER PRIMARY KEY,
    nome TEXT NOT NULL,
    email TEXT NOT NULL,
    senha TEXT NOT NULL
);

-- Cria tabela Reserva
CREATE TABLE Reserva (
    id INTEGER PRIMARY KEY,
    sessao_id INTEGER NOT NULL,
    usuario_id INTEGER NOT NULL,
    cadeira INTEGER NOT NULL,
    FOREIGN KEY(sessao_id) REFERENCES Sessao(id),
    FOREIGN KEY(usuario_id) REFERENCES Usuario(id)
);

-- Cria trigger para inserir 30 cadeiras quando uma nova sala é criada
CREATE TRIGGER insert_cadeiras
AFTER INSERT ON Sala
FOR EACH ROW
BEGIN
    INSERT INTO Cadeiras (sala_id, numero)
    SELECT NEW.id, n FROM (SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 
                            UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9 UNION SELECT 10
                            UNION SELECT 11 UNION SELECT 12 UNION SELECT 13 UNION SELECT 14 UNION SELECT 15
                            UNION SELECT 16 UNION SELECT 17 UNION SELECT 18 UNION SELECT 19 UNION SELECT 20
                            UNION SELECT 21 UNION SELECT 22 UNION SELECT 23 UNION SELECT 24 UNION SELECT 25
                            UNION SELECT 26 UNION SELECT 27 UNION SELECT 28 UNION SELECT 29 UNION SELECT 30) s
    WHERE NOT EXISTS (SELECT * FROM Cadeiras WHERE sala_id = NEW.id AND numero = s.n);
END;

-- Cria trigger para atualizar a disponibilidade da cadeira após uma reserva ser feita
CREATE TRIGGER update_cadeira_disponibilidade
AFTER INSERT ON Reserva
FOR EACH ROW
BEGIN
    UPDATE Cadeiras SET disponivel = 0 WHERE sala_id = NEW.sessao_id AND numero = NEW.cadeira;
END;
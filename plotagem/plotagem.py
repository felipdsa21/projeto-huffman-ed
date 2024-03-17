import matplotlib.pyplot as plt


def ler_numeros(caminho_arquivo: str) -> list[int]:
    with open(caminho_arquivo, "r") as arquivo:
        return sorted(map(int, arquivo))


numeros_lidos_linked = ler_numeros("interacoes_linked.txt")
numeros_lidos_tree = ler_numeros("interacoes_tree.txt")

plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.plot(numeros_lidos_linked, "o")
plt.title("Lista encadeada")
plt.xlabel("Quantidade de elementos")
plt.ylabel("Número de interações")
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(numeros_lidos_tree, "o")
plt.title("Árvore de busca binária")
plt.xlabel("Quantidade de elementos")
plt.ylabel("Número de interações")
plt.grid(True)

plt.tight_layout()
plt.show()

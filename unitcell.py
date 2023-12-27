import numpy as np
import sys


def push(stack, jvec, jnorm):
    """
    既存のベクトルと素かどうかをチェックしろ

    Args:
      jx: x成分
      jy: y成分
      jz: z成分
      t: t成分
      r: 長さ
    """

    nj = jvec @ jvec
    for i in range(len(stack)):
        ivec, _ = stack[i]
        ni = ivec @ ivec
        nij = ivec @ jvec
        if nij * nij == ni * nj:
            if nj < ni:
                stack[i] = jvec, jnorm
            return

    stack.append((jvec, jnorm))


def prime_vectors(ivec, cell, grid_range=10, tolerance=0.02):
    inorm = np.linalg.norm(ivec)

    stack = []
    # find lattice points on the lattice which are perpendicular to vector a
    for jx in range(-grid_range, grid_range + 1):
        for jy in range(-grid_range, grid_range + 1):
            for jz in range(-grid_range, grid_range + 1):
                jvec = np.array([jx, jy, jz])
                rjvec = jvec @ cell
                jnorm = np.linalg.norm(rjvec)
                if jnorm != 0:
                    cosine = abs(ivec @ rjvec) / (inorm * jnorm)
                    if cosine < tolerance:
                        push(stack, jvec, jnorm)
    return stack


def orthogonalize(cell, ivec=np.array([1, 1, 1])):
    # a-vector
    # これも与える必要はなく、全自動で全部計算すれば良いのでは?
    # たぶんGenIce2のunitcell.pyはそれをやっていた。
    # もっとちゃんと調査すれば、重複を減らして、全数探査できると思う。
    tolerance = 0.02

    vectors = prime_vectors(ivec, cell, tolerance=tolerance)

    rivec = ivec @ cell
    inorm = np.linalg.norm(rivec)
    for j in range(len(vectors)):
        jvec, jnorm = vectors[j]
        rjvec = jvec @ cell
        for k in range(j + 1, len(vectors)):
            kvec, knorm = vectors[k]
            rkvec = kvec @ cell
            cosine = abs(rkvec @ rjvec) / (knorm * jnorm)
            if cosine < tolerance:
                volume = abs(np.linalg.det([rivec, rjvec, rkvec]))
                print(
                    int(volume / np.linalg.det(cell)),
                    f"{volume/ (inorm * jnorm * knorm):.6f}",
                    *ivec,
                    *jvec,
                    *kvec,
                )


def main():
    cell = np.diag([float(x) for x in sys.argv[1:]])
    orthogonalize(cell)


if __name__ == "__main__":
    main()

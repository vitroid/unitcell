import numpy as np
import sys
from logging import getLogger, basicConfig, INFO
import itertools as it


def push(primes, jvec):
    nj = jvec @ jvec
    for i in range(len(primes)):
        ivec = primes[i]
        ni = ivec @ ivec
        nij = ivec @ jvec
        # 同軸ベクトルの場合
        if nij * nij == ni * nj:
            # 短い方を採用
            if nj < ni:
                primes[i] = jvec
            # 同じ長さなら、正の要素が多いほうを採用
            elif ni == nj:
                if np.sum(ivec) < np.sum(jvec):
                    primes[i] = jvec
            return primes
    primes.append(jvec)
    return primes


def prime_vectors(grid_range=10):
    """
    この関数は、指定されたグリッド範囲内の素数ベクトルのリストを生成し、それらを大きさに基づいて並べ替えて返します。
    「素数ベクトル」とは、整数ベクトルのうち、それが別の整数ベクトルの整数倍で表せないものを指します。

    Args:
      grid_range: `grid_range` パラメータは、素数ベクトルの各座標の値の範囲を決定します。この場合、これは 10 に設定されています。これは、素数ベクトルの座標の範囲が -10
    から 10 であることを意味します。. Defaults to 10

    Returns:
      関数 `prime_vectors` は、素数ベクトルのソートされたリストを返します。
    """
    logger = getLogger()
    primes = []
    for jx in range(-grid_range, grid_range + 1):
        for jy in range(-grid_range, grid_range + 1):
            for jz in range(-grid_range, grid_range + 1):
                jvec = np.array([jx, jy, jz])
                if jvec @ jvec != 0:
                    primes = push(primes, jvec)
    logger.info(f"{len(primes)} prime vectors are listed.")
    return sorted(primes, key=lambda x: x @ x)


def orthogonalize(cell, tolerance=0.02):
    """
    「orthogonalize」関数は、セルと許容値を入力として受け取り、指定された許容値内で互いに直交する一連の素数ベクトルから 3 つのベクトルを生成します。

    Args:
      cell: 「セル」パラメータは、結晶構造の格子ベクトルを表す行列です。直交性をチェックするために素数ベクトルを変換するために使用されます。
      tolerance: orthogonalize 関数の許容誤差パラメーターは、2 つのベクトルが直交しているとみなされるために許容される最大コサイン値を決定します。 2
    つのベクトル間のコサイン値が許容値より小さい場合、ベクトルは直交していると見なされます。許容誤差のデフォルト値は 0.02 です。
    """
    primes = prime_vectors(grid_range=8)
    for ivec, jvec, kvec in it.combinations(primes, 3):
        rivec = ivec @ cell
        inorm = np.linalg.norm(rivec)
        rjvec = jvec @ cell
        jnorm = np.linalg.norm(rjvec)
        cosine = abs(rivec @ rjvec) / (inorm * jnorm)
        if cosine < tolerance:
            rkvec = kvec @ cell
            knorm = np.linalg.norm(rkvec)
            cosine = abs(rivec @ rkvec) / (inorm * knorm)
            if cosine < tolerance:
                cosine = abs(rjvec @ rkvec) / (jnorm * knorm)
                if cosine < tolerance:
                    yield ivec, jvec, kvec


def main():
    basicConfig(level=INFO)
    cell = np.diag([float(x) for x in sys.argv[1:]])

    for ivec, jvec, kvec in orthogonalize(cell):
        rivec = ivec @ cell
        rjvec = jvec @ cell
        rkvec = kvec @ cell
        inorm = np.linalg.norm(rivec)
        jnorm = np.linalg.norm(rjvec)
        knorm = np.linalg.norm(rkvec)
        volratio = abs(np.linalg.det([ivec, jvec, kvec]))
        volume = abs(np.linalg.det([rivec, rjvec, rkvec]))
        orthogonality = volume / (inorm * jnorm * knorm)
        if 0.999999 < orthogonality:
            print(
                f"{volratio:.06f} {orthogonality:.06f}",
                *ivec,
                *jvec,
                *kvec,
                rivec,
                rjvec,
                rkvec,
            )


if __name__ == "__main__":
    main()

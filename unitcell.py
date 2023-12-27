def push(stack, jx, jy, jz, rj):
    """
    既存のベクトルと素かどうかをチェックしろ

    Args:
      jx: x成分
      jy: y成分
      jz: z成分
      t: t成分
      r: 長さ
    """

    nj = jx * jx + jy * jy + jz * jz
    for i in range(len(stack)):
        ix, iy, iz, _ = stack[i]
        ni = ix * ix + iy * iy + iz * iz
        nij = ix * jx + iy * jy + iz * jz
        if nij * nij == ni * nj:
            if nj < ni:
                stack[i] = jx, jy, jz, rj
            return

    stack.append((jx, jy, jz, rj))


def prime_vectors(ivec, cell, grid_range=10, tolerance=0.02):
    bx, by, bz = cell
    ix, iy, iz = ivec

    ri = (ix**2 + iy**2 + iz**2) ** 0.5

    stack = []
    # find lattice points on the lattice which are perpendicular to vector a
    for jx in range(-grid_range, grid_range + 1):
        for jy in range(-grid_range, grid_range + 1):
            for jz in range(-grid_range, grid_range + 1):
                rjx = jx * bx
                rjy = jy * by
                rjz = jz * bz
                rj = (rjx**2 + rjy**2 + rjz**2) ** 0.5
                if rj != 0:
                    cosine = abs(ix * rjx + iy * rjy + iz * rjz) / (ri * rj)
                    if cosine < tolerance:
                        push(stack, jx, jy, jz, rj)
    return stack


def main():
    # cell shape
    bx, by, bz = 9.046782116679205, 7.8347431355469475, 7.386666666666667
    # a-vector
    # これも与える必要はなく、全自動で全部計算すれば良いのでは?
    # たぶんGenIce2のunitcell.pyはそれをやっていた。
    # もっとちゃんと調査すれば、重複を減らして、全数探査できると思う。
    ix, iy, iz = 1, 1, 1
    tolerance = 0.02

    vectors = prime_vectors((ix, iy, iz), (bx, by, bz), tolerance=tolerance)

    ri = (ix**2 + iy**2 + iz**2) ** 0.5
    for j in range(len(vectors)):
        xj, yj, zj, rj = vectors[j]
        rjx = xj * bx
        rjy = yj * by
        rjz = zj * bz
        for k in range(j + 1, len(vectors)):
            xk, yk, zk, rk = vectors[k]
            rkx = xk * bx
            rky = yk * by
            rkz = zk * bz
            cosine = abs(rkx * rjx + rky * rjy + rkz * rjz) / (rk * rj)
            if cosine < tolerance:
                x = rky * rjz - rkz * rjy
                y = rkz * rjx - rkx * rjz
                z = rkx * rjy - rky * rjx
                v = abs(x * ix + y * iy + z * iz)
                print(
                    int(v / (bx * by * bz)),
                    f"{v / (ri * rj * rk):.6f}",
                    ix,
                    iy,
                    iz,
                    xj,
                    yj,
                    zj,
                    xk,
                    yk,
                    zk,
                )


if __name__ == "__main__":
    main()

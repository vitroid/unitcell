def push(stack, jx, jy, jz, t, rj):
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
        ix, iy, iz, _, _ = stack[i]
        ni = ix * ix + iy * iy + iz * iz
        nij = ix * jx + iy * jy + iz * jz
        if nij * nij == ni * nj:
            if nj < ni:
                stack[i] = jx, jy, jz, t, rj
            return

    stack.append((jx, jy, jz, t, rj))


def main():
    bx, by, bz = 1.0, 1.0, 1.0
    ix, iy, iz = 1, 1, 1
    grid_range = 10
    tolerance = 0.02
    ri = (ix**2 + iy**2 + iz**2) ** 0.5

    stack = []

    for jx in range(-grid_range, grid_range + 1):
        for jy in range(-grid_range, grid_range + 1):
            for jz in range(-grid_range, grid_range + 1):
                rjx = jx * bx
                rjy = jy * by
                rjz = jz * bz
                rj = (rjx**2 + rjy**2 + rjz**2) ** 0.5
                if rj != 0:
                    tj = abs(ix * rjx + iy * rjy + iz * rjz) / (ri * rj)
                    if tj < tolerance:
                        push(stack, jx, jy, jz, tj, rj)

    for j in range(len(stack)):
        xj, yj, zj, tj, rj = stack[j]
        rjx = xj * bx
        rjy = yj * by
        rjz = zj * bz
        for k in range(j + 1, len(stack)):
            xk, yk, zk, tk, rk = stack[k]
            rkx = xk * bx
            rky = yk * by
            rkz = zk * bz
            tjk = abs(rkx * rjx + rky * rjy + rkz * rjz) / (rk * rj)
            if tjk < tolerance:
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

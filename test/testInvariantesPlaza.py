for i in open("./test/InvariantesPlaza"):
    vectorMarcado = [int(j) for j in  i.split(" ")[0:-1] ]
    
    if vectorMarcado[0]+vectorMarcado[4] != 3:
        print("ERROR: plaza[0] + plaza[5] != 3")
        exit()

    if vectorMarcado[1] + vectorMarcado[3] != 2:
        print("ERROR: plaza[4] + plaza[12] + plaza[14] != 1")
        exit()

    if vectorMarcado[3] + vectorMarcado[4] + vectorMarcado[5] != 1:
        print("ERROR: plaza[5] + plaza[13] + plaza[15] != 1")
        exit()

    if vectorMarcado[2] + vectorMarcado[3] + vectorMarcado[4]  + vectorMarcado[6] != 5:
        print("ERROR: plaza[0] + plaza[6]!= 1")
        exit()


print("Los invariantes de plaza son correctos")

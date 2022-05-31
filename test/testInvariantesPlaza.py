for i in open("./test/InvariantesPlaza"):
    vectorMarcado = [int(j) for j in  i.split(" ")[0:-1] ]
    
    if vectorMarcado[0]+vectorMarcado[4] != 3:
        print("ERROR: plaza[0] + plaza[4] != 3")
        exit()

    if vectorMarcado[1] + vectorMarcado[3] != 2:
        print("ERROR: plaza[1] + plaza[3] != 2")
        exit()

    if vectorMarcado[3] + vectorMarcado[4] + vectorMarcado[5] != 1:
        print("ERROR: plaza[3] + plaza[4] + plaza[5] != 1")
        exit()

    if vectorMarcado[2] + vectorMarcado[3] + vectorMarcado[4]  + vectorMarcado[6] != 5:
        print("ERROR: plaza[2] + plaza[3] + plaza[4] + plaza[6]!= 5")
        exit()


print("Los invariantes de plaza son correctos")

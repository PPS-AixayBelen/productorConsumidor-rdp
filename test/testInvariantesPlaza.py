for i in open("./test/InvariantesPlaza"):
    vectorMarcado = [int(j) for j in  i.split(" ")[0:-1] ]
    
    if vectorMarcado[1]+vectorMarcado[7] != 1:
        print("ERROR: plaza[1] + plaza[7] != 1")
        exit()

    if vectorMarcado[4] + vectorMarcado[12] + vectorMarcado[14] != 1:
        print("ERROR: plaza[4] + plaza[12] + plaza[14] != 1")
        exit()

    if vectorMarcado[5] + vectorMarcado[13] + vectorMarcado[15] != 1:
        print("ERROR: plaza[5] + plaza[13] + plaza[15] != 1")
        exit()

    if vectorMarcado[0] + vectorMarcado[6] != 1:
        print("ERROR: plaza[0] + plaza[6]!= 1")
        exit()
    
    if(vectorMarcado[8]+vectorMarcado[9] != 1):
        print("ERROR: plaza[8] + plaza[9] != 1")
        exit()

print("Los invariantes de plaza son correctos")

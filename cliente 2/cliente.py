import socket

def generar_usuario():
    while True:
        try:
            longitud = int(input("Ingrese la longitud del usuario (entre 5 y 15): "))
            if 5 <= longitud <= 15:
                break
            else:
                print("La longitud debe estar entre 5 y 15.")
        except ValueError:
            print("Por favor, ingrese un número válido.")
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 8888))
        mensaje = f'GENERAR_USUARIO {longitud}'
        s.sendall(mensaje.encode())
        data = s.recv(1024)
        print("Respuesta del servidor:", data.decode())

def generar_pass():
    while True:
        try:
            longitud = int(input("Ingrese la longitud de la contraseña (entre 8 y 50): "))
            if 8 <= longitud <= 50:
                break
            else:
                print("La longitud debe estar entre 8 y 50.")
        except ValueError:
            print("Por favor, ingrese un número válido.")
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 8888))
        mensaje = f'GENERAR_PASS {longitud}'
        s.sendall(mensaje.encode())
        data = s.recv(1024)
        print("Respuesta del servidor:", data.decode())

def main():
    while True:
        print("1. Generar Usuario")
        print("2. Generar Pass")
        print("3. Salir")
        opcion = input("Seleccione una opción: ")
        
        if opcion == '1':
            generar_usuario()
        elif opcion == '2':
            generar_pass()
        elif opcion == '3':
            break
        else:
            print("Opción no válida. Intente de nuevo.")

if __name__ == "__main__":
    main()

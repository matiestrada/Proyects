Este proyecto implementa un modelo en C++ para simular un sistema bancario, incluyendo cuentas bancarias con diferentes restricciones y un banco que administra un conjunto de cuentas.

Funcionalidades principales:
*Clase Cuenta:
  Representa una cuenta bancaria con un titular y un saldo. Permite realizar:
  *Depósitos: Aumentan el saldo de la cuenta.
  *Extracciones: Reducen el saldo, respetando las restricciones según el tipo de cuenta.
*Tipos de cuentas:
  *Cuenta corriente: Permite giro en descubierto hasta un tope definido para cada cliente.
  *Caja de ahorro: Limita la cantidad de extracciones mensuales y no permite giro en descubierto.
  *Cuenta universitaria: Restringe las extracciones diarias a un máximo de $100.
*Clase Banco:
  Representa un banco que contiene un conjunto de cuentas. Implementa una funcionalidad para calcular el activo disponible, es decir, la suma total de los saldos de todas las cuentas.

Detalles adicionales:
*Manejo de restricciones específicas para cada tipo de cuenta al realizar operaciones.
*Modularidad en las clases para una fácil extensión o modificación del sistema.

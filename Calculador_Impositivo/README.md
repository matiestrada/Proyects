Este proyecto implementa un calculador impositivo en C++ diseñado para calcular los impuestos de un contribuyente según su categoría y las facturas de servicios que ha registrado.

Funcionalidades principales:
*Configuración del contribuyente ingresando su nombre y categoría: Limitado, Completo o Extendido.
*Registro de facturas con el detalle del servicio, número de factura y monto.
*Cálculo del impuesto total considerando las facturas cuyo número se encuentre dentro de un rango especificado por el usuario.
*Generación de un objeto ReciboDePago que contiene información como: Nombre del contribuyente - Monto total facturado - Monto fijo a pagar ($5 por defecto) - Monto variable calculado según la categoría del contribuyente - Número de la primera y última factura consideradas.

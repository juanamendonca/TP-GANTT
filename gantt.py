import plotly.express as px
import pandas as pd

df1 = pd.DataFrame([
    dict(Task="Diseño de la interfaz (1 points)", Days=2, Category="Desarrollo"),
    dict(Task="Desarrollo del backend (2 points)", Days=3, Category="Pruebas"),
    dict(Task="Configuración del entorno de pruebas (2 points)", Days=1, Category="Pruebas"),
    dict(Task="Ejecución de pruebas unitarias (4 points)<br>(Depends on \"Configuración del entorno de pruebas\")", Days=2, Category="Pruebas", Base=1),
    dict(Task="Implementación de la funcionalidad principal (6 points)<br>(Depends on \"Desarrollo del backend\")", Days=4, Category="Implementación", Base=3),
])

fig1 = px.bar(df1, x="Days", orientation="h", y="Task", title="ProyectoComplejo (Start:2024-04-01) (Maximum Tasks:6)", color='Category', base="Base")
fig1.update_yaxes(autorange="reversed")
fig1.write_html('gantt.html')
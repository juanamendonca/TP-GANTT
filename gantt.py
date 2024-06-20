import plotly.express as px
import pandas as pd

df1 = pd.DataFrame([
    dict(Task="TareaInicial<br>(Minimum length: 2)", Days=3),
    dict(Task="TareaIntermedia<br>(Minimum length: 3)<br>(Depends on \"TareaInicial\")", Days=5, Base=3),
])

fig1 = px.bar(df1, x="Days", orientation="h", y="Task", title="ProyectoPadre", base="Base")
fig1.update_yaxes(autorange="reversed")
df2 = pd.DataFrame([
    dict(Task="TareaHijo1<br>(Minimum length: 2)", Days=3),
    dict(Task="TareaHijo2<br>(Minimum length: 3)<br>(Depends on \"TareaHijo1\")", Days=5, Base=3),
    dict(Task="TareaHijo3<br>(Minimum length: 1)<br>(Depends on \"TareaHijo2\")", Days=2, Base=8),
])

fig2 = px.bar(df2, x="Days", orientation="h", y="Task", title="ProyectoHijo (Depends on \"ProyectoPadre\")", base="Base")
fig2.update_yaxes(autorange="reversed")
df3 = pd.DataFrame([
    dict(Task="tareaUnion<br>(Minimum length: 2)", Days=5),
])

fig3 = px.bar(df3, x="Days", orientation="h", y="Task", title="UnionDeProyectos (With \"ProyectoPadre\", \"ProyectoHijo\")")
fig3.update_yaxes(autorange="reversed")
with open('gantt.html', 'w') as f:
    f.write(fig1.to_html(full_html=False, include_plotlyjs='cdn'))
    f.write(fig2.to_html(full_html=False, include_plotlyjs='cdn'))
    f.write(fig3.to_html(full_html=False, include_plotlyjs='cdn'))

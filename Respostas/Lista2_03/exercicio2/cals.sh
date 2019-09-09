aux=1
for p in $@
do
if [ $aux = 1 ]
then
concatenar=$p
aux=2
else
cal $concatenar $p
aux=1
fi
done
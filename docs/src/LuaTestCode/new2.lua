function permgen (a, n)

    if n == 0 then
       print('1++++++++++++',a)

       printResult(a)
	   print('2++++++++++++',a)

    else

       for i=1,n do

 

           -- put i-th element as the last one

           a[n], a[i] = a[i], a[n]

 
		   print('1============',i,n,a)
           printResult(a)
		   print('2============',i,n,a)

           -- generate all permutations of the other elements

           permgen(a, n - 1)



           -- restore i-th element

           a[n], a[i] = a[i], a[n]
 		   print('1------------',i,n,a)
           printResult(a)
		   print('2------------',i,n,a)
 

       end

    end

end

 

function printResult (a)

    for i,v in ipairs(a) do

       io.write(v, " ")

    end

    io.write("\n")

end

 

permgen ({1,2,3,4}, 4)
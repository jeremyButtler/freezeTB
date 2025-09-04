" import functions with `source file.vim`

" incurments 2 digit functions nubmers by amount
function! IncTwoDigFun(start, end, amount)
   let l:funNum = a:end
   let l:cursorPos = winsaveview()
      " save cursor to position

   while l:funNum >= a:start
      let l:nextNum = l:funNum + a:amount

      if l:funNum > 9
         let l:subStr = 'un'.l:funNum
      else
         let l:subStr = 'un0'.l:funNum
      endif

      if l:nextNum > 9
         let l:repStr = 'un'.l:nextNum
      else
         let l:repStr = 'un0'.l:nextNum
      endif

      let l:lineNum = 1

      " not very efficent, but best way to do search and
      " replace when using variables
      " for hardcoding you can use `%s/sub/replace/g`

      for l:lineStr in getline(1,"$")
         call setline(l:lineNum, substitute(l:lineStr, l:subStr, l:repStr, "g"))
         let l:lineNum = l:lineNum + 1
      endfor

      let l:funNum = l:funNum - 1
   endwhile

   call winrestview(l:cursorPos)
      " move cursor back to orignal position
endfunction

" incurments 3 digit functions nubmers by amount
function! IncThreeDigFun(start, end, amount)
   let l:funNum = a:end
   let l:cursorPos = winsaveview()
      " save cursor to position

   while l:funNum >= a:start
      let l:nextNum = l:funNum + a:amount

      if l:funNum > 99
         let l:subStr = 'un'.l:funNum
      elseif l:funNum > 9
         let l:subStr = 'un0'.l:funNum
      else
         let l:subStr = 'un00'.l:funNum
      endif

      if l:nextNum > 99
         let l:repStr = 'un'.l:nextNum
      elseif l:nextNum > 9
         let l:repStr = 'un0'.l:nextNum
      else
         let l:repStr = 'un00'.l:nextNum
      endif

      let l:lineNum = 1

      " not very efficent, but best way to do search and
      " replace when using variables
      " for hardcoding you can use `%s/sub/replace/g`

      for l:lineStr in getline(1,"$")
         call setline(l:lineNum, substitute(l:lineStr, l:subStr, l:repStr, "g"))
         let l:lineNum = l:lineNum + 1
      endfor

      let l:funNum = l:funNum - 1
   endwhile

   call winrestview(l:cursorPos)
      " move cursor back to orignal position
endfunction

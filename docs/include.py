
# Replace |br| with line break
rst_prolog += r'''
.. |br| raw:: html

   <br/>
'''
# Start overview (collapsible section with the summary 'Overview')
rst_prolog += r'''
.. |startovw| raw:: html

   <details><summary><a><b> Overview </b></a></summary><br/>
'''
# End collapsible section
rst_prolog += r'''
.. |endcollapse| raw:: html

   </details>
'''

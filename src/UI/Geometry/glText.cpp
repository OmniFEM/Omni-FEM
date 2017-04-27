#include <UI/ModelDefinition/glText.h>


struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

glText::glText(physicProblems problem)
{
    
    glewExperimental = GL_TRUE;
    glewInit();
    _problem = problem;
    
    FT_Library font;
    
    if(FT_Init_FreeType(&font))
    {
        // Throw an error if there is something wrong
        return;
    }
    
    FT_Face face;
    
    // Todo: Repalce this file path with a more universal file path to ensure cross-distro compatibility
    if(FT_New_Face(font, "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 0, &face))
    {
        // Error if something went wrong
        return;
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    GLuint texture;
    

//	glUniform1i(uniform_tex, 0);
    
    for(GLubyte c = 0; c < 128; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            // Error if something happens
            return;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
       
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        

          
        
        character addChar = {
            texture, 
            wxSize(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            wxSize(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        
        _characterList.insert(std::pair<GLchar, character>(c, addChar));
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(font);
    
 /*   glEnable(GL_CULL_FACE);
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
     */ 
     
}



void glText::renderBlockLabelText(blockLabel &label)
{
/*    GLfloat scale = 1.0;
    GLfloat xplace = 0.25;
    GLfloat yPlace = 0.25;
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
 //   glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    std::string text = label.getProperty()->getMaterialName();
    for (c = text.begin(); c != text.end(); c++) 
    {
        glColor3f(0.0, 0.0, 0.0);
        character ch = _characterList[*c];
        
        GLfloat xpos = (label.getCenterXCoordinate() + 0.25) + ch.glyphBearing.GetX() * scale;
        GLfloat ypos = (label.getCenterYCoordinate() + 0.25) - (ch.glyphSize.GetY() - ch.glyphBearing.GetY()) * scale;

        GLfloat w = ch.glyphSize.GetX() * scale;
        GLfloat h = ch.glyphSize.GetY() * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
   //     glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
   //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
   //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        xplace += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
*/    
}



void glText::renderBlockLabelText()
{
    GLfloat scale = 1.0;
    
    GLfloat xplace = 0.25;
    GLfloat yPlace = 0.25;
    
        glActiveTexture(GL_TEXTURE0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glColor3f(0.0, 0.0, 0.0); 
    
 //   glBindVertexArray(_VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    std::string text = "te";
    for (c = text.begin(); c != text.end(); c++) 
    {

        int asciiNumber = *c;
        
        character ch = _characterList[asciiNumber];
        
  //      glBindTexture(GL_TEXTURE_RECTANGLE, ch.textureID);

        GLfloat xpos = (xplace) + ch.glyphBearing.GetX() * scale;
        GLfloat ypos = (yPlace) - (ch.glyphSize.GetY() - ch.glyphBearing.GetY()) * scale;

        GLfloat w = ch.glyphSize.GetX() * scale;
        GLfloat h = ch.glyphSize.GetY() * scale;

  /*      glBegin(GL_QUADS);
            glTexCoord2f (xpos, ypos + h);
            glVertex3f (xpos, ypos + h, 0.0);
            glTexCoord2f (xpos, ypos);
            glVertex3f (xpos, ypos, 0.0);
            glTexCoord2f (xpos + w, ypos);
            glVertex3f (xpos + w, ypos, 0.0);
            glTexCoord2f (xpos + w, ypos + h);
            glVertex3f (xpos + w, ypos + h, 0.0);
        glEnd();
         */ 
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    /*    
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);*/
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        xplace += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_BLEND);    
 //   glDisable(GL_TEXTURE_2D);    
}
<GameFile>
  <PropertyGroup Name="LoginScene" Type="Layer" ID="3181b748-f9d7-42c7-bf0a-9f75473feb6b" Version="2.3.3.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" ctype="GameLayerObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="Root" ActionTag="2043803611" Tag="2" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="PanelLogin" ActionTag="-1968791102" Tag="3" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BothEdge" LeftMargin="580.0000" TouchEnable="True" StretchHeightEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="380.0000" Y="640.0000" />
                <Children>
                  <AbstractNodeData Name="ImageLogin" ActionTag="401178041" Tag="4" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="107.5000" RightMargin="107.5000" TopMargin="94.7001" BottomMargin="504.2999" Scale9Width="165" Scale9Height="41" ctype="ImageViewObjectData">
                    <Size X="165.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="524.7999" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.8200" />
                    <PreSize X="0.4342" Y="0.0641" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/titile_login.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImageUserName" ActionTag="393408449" Tag="5" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="187.2000" BottomMargin="404.8000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldUserName" ActionTag="-393339220" Tag="11" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Your User Name" MaxLengthEnable="True" MaxLengthText="20" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="-132761399" VisibleForFrame="False" Tag="43" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="428.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.6700" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImagePassword" ActionTag="-176744670" Tag="6" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="264.0000" BottomMargin="328.0000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldPassword" ActionTag="1619722127" CallBackType="Event" Tag="16" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Password" MaxLengthEnable="True" MaxLengthText="20" PasswordEnable="True" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="2094213364" VisibleForFrame="False" Tag="44" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="352.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5500" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="TextNotify" ActionTag="1953734185" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="190.0000" RightMargin="190.0000" TopMargin="332.8000" BottomMargin="307.2000" FontSize="20" LabelText="&#xA;&#xA;" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="307.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="203" G="47" B="47" />
                    <PrePosition X="0.5000" Y="0.4800" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonPlayNow" ActionTag="-1299820821" Tag="7" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="38.0000" RightMargin="38.0000" TopMargin="361.5000" BottomMargin="201.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="274" Scale9Height="55" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="304.0000" Y="77.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="342.0000" Y="240.0000" />
                    <Scale ScaleX="0.9211" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9000" Y="0.3750" />
                    <PreSize X="0.8000" Y="0.1203" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_play.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_play.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonFacebook" ActionTag="1494493644" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="270.2520" RightMargin="39.7480" TopMargin="443.8000" BottomMargin="130.2000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="40" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="70.0000" Y="66.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="340.2520" Y="163.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8954" Y="0.2550" />
                    <PreSize X="0.1842" Y="0.1031" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_facebook.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_facebook.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonRegister" ActionTag="-1292607892" Tag="9" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="145.0000" RightMargin="38.0000" TopMargin="516.6000" BottomMargin="49.4000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="167" Scale9Height="52" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="197.0000" Y="74.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="342.0000" Y="86.4000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9000" Y="0.1350" />
                    <PreSize X="0.5184" Y="0.1156" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_register.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_register.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="TextFogotPassword" ActionTag="-1996710066" Tag="10" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="6.0000" RightMargin="6.0000" TopMargin="596.5000" BottomMargin="20.5000" TouchEnable="True" FontSize="20" LabelText="Forgot password? (Only register account)" TouchScaleChangeAble="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="368.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="32.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.0500" />
                    <PreSize X="0.9684" Y="0.0359" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="960.0000" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="0.5000" />
                <PreSize X="0.3958" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="PanelRegister" ActionTag="1172700924" VisibleForFrame="False" Tag="17" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="581.7924" RightMargin="-1.7924" TouchEnable="True" StretchHeightEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="380.0000" Y="640.0000" />
                <Children>
                  <AbstractNodeData Name="ImageRegister" ActionTag="1289411921" Tag="19" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="88.0000" RightMargin="88.0000" TopMargin="75.4999" BottomMargin="523.5001" Scale9Width="204" Scale9Height="41" ctype="ImageViewObjectData">
                    <Size X="204.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="544.0001" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.8500" />
                    <PreSize X="0.5368" Y="0.0641" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/title_register.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImageUserName" ActionTag="2117905408" Tag="21" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="155.2000" BottomMargin="436.8000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldUserName" ActionTag="1428953890" Tag="20" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Your User Name" MaxLengthText="10" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="-1866457879" VisibleForFrame="False" Tag="45" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="460.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.7200" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="TextNotifyUserName" ActionTag="-251757115" Tag="26" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="114.5000" RightMargin="114.5000" TopMargin="206.1000" BottomMargin="410.9000" FontSize="20" LabelText="Thấp nhất 6 kí tự" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="151.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="422.4000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.6600" />
                    <PreSize X="0.3974" Y="0.0359" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImagePassword" ActionTag="-691338557" Tag="23" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="238.4000" BottomMargin="353.6000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldPassword" ActionTag="974265100" Tag="22" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Password" MaxLengthEnable="True" MaxLengthText="20" PasswordEnable="True" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="-503407274" VisibleForFrame="False" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="377.6000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5900" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="TextNotifyPassword" ActionTag="-1656202957" Tag="27" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="114.5000" RightMargin="114.5000" TopMargin="289.3000" BottomMargin="327.7000" FontSize="20" LabelText="Thấp nhất 6 kí tự" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="151.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="339.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5300" />
                    <PreSize X="0.3974" Y="0.0359" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImageConfirmPassword" ActionTag="855656694" Tag="28" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="321.6000" BottomMargin="270.4000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldConfirmPassword" ActionTag="-1534016939" Tag="29" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Confirm Password" MaxLengthEnable="True" MaxLengthText="20" PasswordEnable="True" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="1716381413" VisibleForFrame="False" Tag="47" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="294.4000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4600" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="TextNotifyConfirmPassword" ActionTag="-1771010388" VisibleForFrame="False" Tag="44" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="63.0000" RightMargin="63.0000" TopMargin="372.5000" BottomMargin="244.5000" FontSize="20" LabelText="Text Notify ConfirmPassword" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="254.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="256.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4000" />
                    <PreSize X="0.6684" Y="0.0359" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImageEmail" ActionTag="1501469624" Tag="30" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="404.8000" BottomMargin="187.2000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldEmail" ActionTag="194695783" Tag="31" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="-1.7925" BottomMargin="1.7925" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Your email" MaxLengthEnable="True" MaxLengthText="25" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="2046806213" VisibleForFrame="False" Tag="48" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="25.7925" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5373" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="211.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.3300" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="TextNotifyEmail" ActionTag="-110440183" VisibleForFrame="False" Tag="45" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="117.0000" RightMargin="117.0000" TopMargin="455.7000" BottomMargin="161.3000" FontSize="20" LabelText="Text Nofify Email" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="146.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="172.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2700" />
                    <PreSize X="0.3842" Y="0.0359" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonRegister" ActionTag="430865918" Tag="32" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="91.5000" RightMargin="91.5000" TopMargin="487.8000" BottomMargin="78.2000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="167" Scale9Height="52" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="197.0000" Y="74.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="115.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.1800" />
                    <PreSize X="0.5184" Y="0.1156" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_register.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_register.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonClose" ActionTag="-111301894" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-32.5000" RightMargin="347.5000" TopMargin="19.2000" BottomMargin="556.8000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="35" Scale9Height="42" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="65.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position Y="588.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.9200" />
                    <PreSize X="0.1711" Y="0.1000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_close.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_close.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="961.7924" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0019" Y="0.5000" />
                <PreSize X="0.3958" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="PanelForgetPassword" ActionTag="-1502357113" VisibleForFrame="False" Tag="34" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="580.0000" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="380.0000" Y="640.0000" />
                <Children>
                  <AbstractNodeData Name="ImageForgetPassword" ActionTag="-559934141" Tag="35" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="45.4346" RightMargin="49.5654" TopMargin="97.1348" BottomMargin="510.8652" Scale9Width="285" Scale9Height="32" ctype="ImageViewObjectData">
                    <Size X="285.0000" Y="32.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="187.9346" Y="526.8652" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4946" Y="0.8232" />
                    <PreSize X="0.7500" Y="0.0500" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/titile_forgetpassword.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImageUserName" ActionTag="-1672013412" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="155.2000" BottomMargin="436.8000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldUserName" ActionTag="-1126414208" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Your User Name" MaxLengthEnable="True" MaxLengthText="20" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="1119419569" VisibleForFrame="False" Tag="49" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="460.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.7200" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ImageEmail" ActionTag="532841846" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="232.0000" BottomMargin="360.0000" Scale9Width="282" Scale9Height="48" ctype="ImageViewObjectData">
                    <Size X="282.0000" Y="48.0000" />
                    <Children>
                      <AbstractNodeData Name="TextFieldEmail" ActionTag="719725068" Tag="39" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Your Email" MaxLengthEnable="True" MaxLengthText="20" ctype="TextFieldObjectData">
                        <Size X="282.0000" Y="48.0000" />
                        <Children>
                          <AbstractNodeData Name="ImageTick" ActionTag="1838558793" VisibleForFrame="False" Tag="50" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2300" RightMargin="275.7700" TopMargin="3.0000" BottomMargin="3.0000" Scale9Width="4" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="4.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="4.2300" Y="24.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0150" Y="0.5000" />
                            <PreSize X="0.0142" Y="0.8750" />
                            <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/ImageTick.png" Plist="ui/SpriteSheetLoginScene.plist" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="141.0000" Y="24.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="384.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.6000" />
                    <PreSize X="0.7421" Y="0.0750" />
                    <FileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/textfield_input.png" Plist="ui/SpriteSheetLoginScene.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonSubmit" ActionTag="-2111295189" Tag="40" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="91.5000" RightMargin="91.5000" TopMargin="315.0001" BottomMargin="250.9999" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="167" Scale9Height="52" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="197.0000" Y="74.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="190.0000" Y="287.9999" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4500" />
                    <PreSize X="0.5184" Y="0.1156" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_submit.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_submit.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="ButtonClose" ActionTag="1660239241" Tag="41" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-32.5000" RightMargin="347.5000" TopMargin="19.2000" BottomMargin="556.8000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="35" Scale9Height="42" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="65.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position Y="588.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.9200" />
                    <PreSize X="0.1711" Y="0.1000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_close.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <NormalFileData Type="MarkedSubImage" Path="ui/ImagesLoginScene/button_close.png" Plist="ui/SpriteSheetLoginScene.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="960.0000" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="0.5000" />
                <PreSize X="0.3958" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="480.0000" Y="320.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>
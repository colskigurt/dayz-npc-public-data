# 현재 커스텀 모델의 동물 네이티브 Move 연결 — 1개체 능력 진단

## 1. 작업 목표
기존연구를재사용하고검사를무작정줄이는대신현재AnimalBase 커스텀모델에서네이티브Move명령을실행할수있는지확인한다. 원본NPC파이프라인을보존한단독능력시험이다. CPU최적화·충돌동등성·전체플레이가능NPC의구현성공으로확대하지않는다.

## 2. 실제 수행
Combined-Proposal은중간Contacts/lift와부분성공rollback계약때문에기각된자료를확인했다. Scratch의국소1.5~2.5%미달,회전산술65536좌표동등/국소메서드통과·전체효과미측정,1mm범위후보미수용,33ms철회를재사용했다. 같은시험을반복하지않았다.
설치SDK의DayZAnimal.StartCommand_Move,속도·heading입력,AnimCommandBase.GetEntity/PreAnim_SetInt와MoveModdable선언을읽었다. 현재13PBO의구성과설정을대조해Animal_NFWeighted224Full→Animal_NFPaletteFull_reference0의graph/instance연결을확인했다.
별도PBO의한개체NMPEnabled표식에서만기존HandleNativeCommand를설치기본handler에넘겼다. MoveModdable.PreAnimUpdate에서기존RequestedState를0/1/2로전달하고물리·애니메이션콜백을계수했다. 현재전체NPC의DriveGround/Displace/지지·충돌코드는변경하지않았다.
초기World/미션컴파일은기존modded메서드의인자명을다르게쓴오류로실패했다. 원래인자명으로고친별도revision2 PBO를서명/검증했다. revision3는접속/컴파일통과후준비이벤트가안들어와NPC생성없이300초예산이끝났다. 화면에서플레이어접속은확인했으나NPC동작으로세지않았다. 클라이언트정상닫기·서버소유자확인후강제정리영수증을남겼다.
revision4는기존씬처럼CreateCharacter에서Visitor를연결해같은PBO로20게임초능력시험을완료했다. 원래14클립인스턴스를사용하며실행한요청은idle/walk/run뿐이다. 새ETW·영상·큰덤프·성능벤치는없다.

## 3. 측정 결과
현재단일렌더Animal_NFWeighted224Full1개,별도피해수신체0·GroundBody미연결·추적AI미연결인능력프로브다. 기존1논리NPC+렌더+수신체의비용비교대상이아니다.
20게임초동안40행,관찰창 0.001999~19.925초. 네이티브Move의OnActivate1회,PreAnim/PrePhys각404회,RequestedState변수11바인딩. script명령은관찰행모두없음(로그0),생존1/1. handler raw command값은초기−1과51이며SDK enum의기본정수순번으로추측해매핑하지않는다. MoveModdable의실제콜백을관찰한것을근거로한다.
runtime graph=dg_lab_inputs/native_traversal.agr, instance=dg_lab_inputs/native_combat.asi.
| 단계 | 관찰 구간(게임초) | 수평 순이동(m) | 수평 경로 길이(m) |
|---|---:|---:|---:|
| initial-idle | 0.002~4.586 | 0.0672 | 0.4767 |
| walk-input | 5.100~10.726 | 11.5766 | 11.6643 |
| run-input | 11.246~16.882 | 27.6387 | 27.6477 |
| final-idle | 17.382~19.925 | 0.0981 | 0.2118 |

속도입력1/2의관찰구간수평순이동평균은약2.058/4.904m/게임초다. 입력값을m/s로가정하지않는다. heading입력0에서주이동방향은−Z였으므로현재커스텀heading/표시방향과의보정이남았다. idle에서도작은root이동이있어완전정지계약을통과했다고하지않는다.
CPU·Present p50/p95/p99·물리query수·self-time·실제동영상부드러움미측정. 콜백404회는렌더표시프레임404회나시각품질통과가아니다.

## 4. 원인 분석
확인: 선언만있던동물용Move의script hook이현재커스텀Animal에서실행되고,기존그래프RequestedState입력과실제위치이동이연결된다. 따라서현재모델에서는nativeMove를전혀사용할수없다는전제는성립하지않는다.
추정: 향후평지FOLLOW의이동·접촉소유권을native로넘기면일부script DriveGround/primary query작업을대체할가능성이있다. native내부에같은또는다른검사가추가되므로전체비용이낮아진다는증거는아니다.
미확인: 네이티브지지/contact생산자·캐시·형상·검사빈도·비용,정상밀침/낙하/착지와동등성,피격·공격·사망/수신체동기화,script/native전환시루트·lift·Contacts·graph시계보존,64/200비용과실제시각품질. 이번프로브는이문제들을해결하지않았다.
새방향은작은상수튜닝이아닌이동권한변경후보다. 기존기능이있는본체에바로켜지않으며단독성공을8/64기능통과로바꾸지않는다.

## 5. 검증 수준
SDK/packed설정조회·서명·컴파일·현재모델nativeMove콜백/그래프입력/실제이동:진단성공. 하이브리드구현·추적/등반/낙하/피격/사망회귀·반복성능·시각·최종수용:미실행.20ms p95는목표다. 앞선컴파일실패와시작실패는독립성능반복으로세지않는다.

## 6. 변경 사항
기존rate100-isolation-20261112 아래SDK/packedconfig검토와native-animal-move1의격리소스·미션·계약·영수증·40행원시/분석만추가했다. 원본13PBO·그래프·모델·게임·운영서버·개인키·기존기능/실패증거보존. 사용PBO SHA256 5d7184ef91b44ff4a4a5ffb0ccca918cf20b46a75324d24af78cabc49f651db8,서명·서명검증성공,packed script바이트일치. revision4는revision2PBO재사용·미션만수정했다.
완료시험서버retained exit−1/강제종료없음,클라이언트정상닫기성공. 서버exit0로보고하지않는다. 이전컴파일/준비실패의강제정리는별도영수증으로구분했다. 현재모든시험게임·서버·수집프로세스종료확인.

## 7. 원시 증거
성공능력실행ID NATIVE-MOVE-one-capability1-r4. native-animal-move-feasibility.json의SDK5엔트리/해시, native-animal-packed-config-review.json의설정체인,각revision의계약·소스·서명·실패종료영수증과revision4 native-move-probe.tsv40행/analysis.json/heading-observation.json/server-terminal/client-closed를연결한다. 13원본PBO·client/server해시를실행전에대조했다.
공개는선정원문·40행관측·집계와해시이며전체설치자산·SDK PBO·로컬절대경로·프로세스정보·키는노출하지않는다. 직접화면관찰은revision3의접속확인에한정하며성공revision4애니메이션시각평가가아니다.

## 8. 잔여 문제
다음단일산출물은native지상↔script등반/낙하의권한전환계약이다. 실제root/heading·좌표커밋·중간접촉이력·minimumLift·부분실패·피격중단·수신체·graph진행률을누가소유하는지명시하고,기존계약을보존하지못하면전체NPC이식으로확대하지않는다.
현재사용자가확인한애니메이션경로와기능을보존한다. 원래전체Drive/접촉비용최적화의목표를유지하되이번능력시험을CPU절감성과로보고하지않는다. 다른nativeproxy/엔진결합/대규모LOD설계로자동확대하지않으며,안전한전환후보가있을때만소수개체비교후8→64반복으로간다.

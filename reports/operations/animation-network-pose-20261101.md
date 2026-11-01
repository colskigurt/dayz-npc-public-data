# 네트워크6점 포즈 대응: 본핸들 계측 수정·연속성 gate 미통과

작성: 2026-11-01T16:27:28.041560+09:00

## 1. 작업 목표

실제 mode2 클라이언트가 서버와 대응하는 본 포즈를 받는지 확인한다. 그래프 시계 진행만으로 동작 동등성을 판단하지 않고, 기존 본 조회 계약과 행렬 접근 기록을 재사용한다.

## 2. 실제 수행

변경 없는453D/0C6E/B8FB 자산과 성공한 격리 연결 형식으로1개65본 물리 part를2회 실행했다. 서버는 고정run request2·루트 이동OFF이며6개 이름의GetBoneIndexByName 반환값과GetBonePositionMS를 기록했다. client는기존query188/interpolated190의65개 행렬translation을매3scheduler호출후읽었다. 순수비용시험이아니며native값은쓰지않았다.

첫 계측에서 API 반환값을 배열 ordinal로 간주하고 음수면 누락으로 판단한 전제 오류가 드러났다. 기존 실행 소스뿐 아니라 현재 서명된453D PBO의DGBNativeGround.c에도 이미opaque signed ID/-1 sentinel 계약이 있었다. 새 계측에서 이 기존 계약을 놓친 것이며 엔진의 새로운 결함이나 실행본 수정으로 보고하지 않는다. 첫RightHand 좌표0 및 잘못된ordinal비교는 제외했다.

두 번째 계측은 이 fixture만!=-1 조건으로 고치고,client의실제 이름 해시 테이블에서matrix slot을 읽었다. client PE의Animal GetBoneIndexByName→0x140b0f9d0,hash 조회0x1402e6a30,GetBonePositionMS의virtual+0x610→query188 lookup 경로를 제한적으로 대조했다. 첫 짧은 정적 창의 외부 분기는 후속 확장 JSON에 보존했으며 완전한 전체CFG라고 부르지 않는다.

## 3. 측정 결과

첫 실행은32scheduler호출·10pose표본·유효0.4968012초,수정 계측은369호출·123표본·유효7.2863136초다. 두관찰기 모두오류0·numeric override0이며두server는DONE 뒤소유프로세스만닫았다. 두client RPTtermination·프로세스부재,관찰기종료,crashlog0을확인했다. 기존retirement계약검증은아니다.

수정 실행에서명시된6개이름모두livehashmap에있었다. 실제slot은Head31/Hips3/spine2 13/Neck22/LeftHand29/RightHand64다. RightHand의signed handle은-1405358722이지만누락값-1이아니다.6개SDK반환값모두ASCII이름의확인된hash식과일치했다. 테이블용량128과값범위0~64를검증했다.

서버2151행은모두request2다. client의6개translation을각서버행전체에서가장가까운6점포즈에대응시켰다. interpolated190의123표본중67개가RMS2마이크로미터이내였다. 최근접RMS p50=0.000000531752m,p95=0.0215241m,p99=0.0240739m,max=0.0256175m다. 극소오차는서버TSV6자리반올림과같은포즈의복제에양립하며원본게임정확도나시각품질의증거가아니다.

이것은위상자유최근접shape비교다. 공유시계로같은시점을연결하지않았으므로지연·시간동등성·프레임별동일성을증명하지않는다. query188과interpolated190에서선택한6점은123표본모두같았으나alias/동일typedata를읽은것은아니다. 이름map은190에서읽었으므로188의같은ordinal사용은보강근거가있는가정으로남긴다.

관찰기는선택뒤25초를기다렸지만실제pose진행은각0.50/7.29초에서끝났고server에는world logout이기록됐다. 따라서25초연속gate는실패다. 해제이유는현재로그에서확정하지못했다. client RequestedState직접값·65본회전/전체포즈·화면은미측정이다.8/64회귀·frame-time p50/p95/p99·CPU/GPU·GC도미측정이며20ms p95는목표다.

## 4. 원인 분석

확인: 본이름API는실제배열순서가아닌name-hashhandle을반환한다. 이름존재를검사한뒤hash를반환하고,좌표API는별도조회로matrix slot을찾는다. 이경로와live map을연결해잘못된인덱스추정을제거했다. 기존실행본은이미이계약을지켰고이번수정은추가계측에만해당한다.

확인: 서버run의여섯위치와정확히대응하는client포즈표본을얻었다. 단순clock만움직이고좌표는전부정지라는설명은이유효구간에는맞지않는다. 반면전체포즈·실제요청변수·모든시점의동등성은미입증이다.

미확인: 중간world이탈이인증·연결상태·서버추가본조회·clientmatrix계측중무엇과관계있는지모른다. 기존baseline은25초진행했고이번2회는짧았다는차이만으로계측이원인이라고단정하지않는다. 로그의해제시각·마지막callback·화면오류를연결할추가근거가필요하다.

## 5. 검증 수준

본핸들→slot계약과부분6점pose대응은진단성공이다. 연속성gate는실패이며전체pose/요청동등성·시각품질·성능개선·반복수용·최종수용은미완료다. 이결과를새8/64회귀통과나애니메이션끊김해결로표현하지않는다.

## 6. 변경 사항

격리미션의추가본계측과관찰기·분석본만추가/수정했다. 기존게임코드·PBO·ANM·운영서버·9350후보는보존했다.3개기존PBO해시·서명은두실행전모두검증성공,새PBO/새서명대상은없다. 영상·대용량dump·도구설치는없다. 두실행의원시기록과실패한비교전제도보존한다.

## 7. 원시 증거

분석ID animation-network-pose-20261101-1616와lookup-trial. plan/preflight-change,serverposeTSV,network-observer.json/js,pose-nearest-joins.json,results,본API선언·binding·확장lookup·현재packedhandle계약을연결한다. native관찰은기존PythonFrida,정적분석은Capstone이며REA runtime Evidence가아니다.

공개패킷에는hash/slot표·선정6점대응·관련native함수문맥·실패범위를제공하고실제PID/프로세스/CLI·ASLRbase·전체로그·사용자식별자·로컬경로·키·자산은제외한다. 종료·JSON·해시·파일안정성을검증한후기록한다.

## 8. 잔여 문제

다음은새포즈수용시험보다world연속성진단이다. 마지막실제callback시각을보존하고world이탈직후오류화면/해제이유를자동창닫기보다먼저확보해야한다. 기존성공baseline과추가server본조회/clientmatrix읽기를독립factor로구분한다. 같은짧은시험을원인구분없이반복하지않는다.

연속성확보후에는이번검증된hashmap을재사용하고양쪽typedata매핑/동기시각·회전/표시품질을추가확인한다. 전체NPC권한이식·새보간·native시간쓰기·속도배율은보류한다. 해시/서명불일치·crash·관찰오류·대상이탈은중단조건이며64개체비용실험은별도로유지한다.

선정 공개 본문. 로컬 보고서 SHA-256: `4780dcabb70226293cf1049123bb94b94ea12f486eff1117c56f292ab2380973`. 원시 전체가 아니다.

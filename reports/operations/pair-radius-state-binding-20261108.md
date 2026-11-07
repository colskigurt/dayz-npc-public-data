# 접촉 후보: 상태512 동등성 및 기본OFF 서명 모듈

## 1. 작업 목표
국소 비용 반복 통과 후보의 전투·관리형 등반 상태를 보완하고 격리 네트워크 시험을 준비.

## 2. 실제 수행
기존244입력에 Combat Active/Dead/PendingDeath/PendingHit/Reacting, managed/unmanaged MOTION CLIMB 및 managed DROP을 추가하여 실제 Enforce서버에서512입력 비교.
검증된 pair method를 기본OFF modded DGBNativeCrowd로 연결. policy guard를 제거하면 시험 수식과 정확히 일치하도록 확인. 기존 빌드 절차를 재사용해 격리PBO 빌드·서명·검증.8OFF/ON60초 설정은 준비만 했고 미실행.

## 3. 측정 결과
512행 exact mismatch0. 논리64/native entity0. 서버 요청 종료 뒤 actualexit-1, 강제 종료 없음.
서명 검증 성공, 개인키 포함 없음. 추가 CPU·Present·실제 표시 품질 미측정.

## 4. 원인 분석
검사된 논리 상태의 힘·속도·접촉수 결과는 동일했다. native 월드 움직임·실제 피격은 포함하지 않음.
최초pair에서 정책을 읽고 OFF는 원본 위임, ON은 검증한 수식 사용. 정책분기 오버헤드는 향후 전체CPU 비교에 포함해야 한다.

## 5. 검증 수준
상태 메서드 동등성 진단 및 빌드/서명 검증 성공. 모듈 자체 Enforce컴파일·네트워크런타임·8/64실제 기능·시각·전체성능 수용 미완료.20ms달성 아님.

## 6. 변경 사항
격리 소스/출력/8시험설정만 추가. 기존 게임소스·운영PBO 불변.
PBO SHA256 A226824ED32670C0B4145F3D2F2A1B2F8FEDACBE0FFAF40A47E57E2DB65573BF.
기존키 읽기전용, 개인키 생성·복사·배포 없음.

## 7. 원시 증거
ANIM-pair-shared-state-matrix1. 내부 grid-method.tsv·analysis·plan·로그·종료, binding-proof, build/signature 및8계약/설정 보존. 저장 전 해시 검증. 민감 원시 자료 공개 없음.

## 8. 잔여 문제
8OFF/ON60초 통합과64전체CPU 미실행.60초통합은 full mobility gate를 대체하지 않음. 기존 입구정체 결함 별도 유지.
기존 클라이언트 observer는320기준이라8에서는1Hz열거가 계속됨:8클라이언트 성능 근거로 사용 금지. 실제 피격·사망·시각 최종검증 남음.

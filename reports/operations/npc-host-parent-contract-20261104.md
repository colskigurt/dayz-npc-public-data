# 8파트 transform 전파 후보: 최소 계약 실패

## 1. 작업 목표

64개체 CPU 비용 최적화의 다음 구조 후보로, 8파트별 위치 쓰기를 공식 부모–자식 transform 전파로 대체할 수 있는지 최소 계약을 확인한다. 호스트 통합·물리 해제·이동 솔버 병합은 하지 않는다.

## 2. 실제 수행

기존 8파트 분할·NOCOLLISION 실패·네트워크 부모 읽기 연구를 재사용했다. 과거 부모 연구는 GetParent/ID 읽기와 바인딩 분석이었고 이번 Animated Animal 간 AddChild의 동작 수용을 입증한 자료는 아니었다. 공식 enentity.c AddChild(child,-1,false), RemoveChild(child,true) 계약을 확인했다.

격리 2파트에 기존 물리와 독립 애니메이션을 유지하고 1초 baseline 후 연결했다. 연결 직후 자식 local origin/angles를 0으로 설정한 뒤 부모만 이동·회전했다. 원본 게임 코드·PBO는 변경하지 않았다. 최초 contract1은 opaque dBlock의 ref 표기와 OnUpdate 인자명 오류로 compile 실패했다. 1b는 그 두 문법만 고쳤다. 두 원본 기록 모두 보존했다.

## 3. 측정 결과

|항목|결과|
|---|---:|
|전체 관측 행|58|
|연결 후 관측 행|10|
|AddChild 반환·부모 identity|연결 후 true|
|초기 연결 root 오차|0m|
|마지막 root 오차|11202.9m|
|연결 직전/실패 시 native animation callback 수|양쪽 4→5|
|실패 시 physics body 존재|양쪽 true|

큰 좌표 이탈은 정상 이동 기능이 아니라 계약 실패다. 같은 표본에서 두 파트의 head MS pose와 native seconds가 갱신돼 단순 포즈 정지와 구별된다. 성능 p50/p95/p99·CPU Busy·실제 화면 품질은 미측정. 기존 8PBO 서명 검증은 성공했다. 후보 PBO 없음.

## 4. 원인 분석

확인: 공식 부모 연결이 성립해도 실행 중 world transform 보존은 보장되지 않았다. 실패는 callback 수 증가와 같은 표본에서 관찰됐다. 추정: 네이티브 Animal 이동/물리와 hierarchy 좌표 처리의 결합 문제일 수 있다. 정확한 native 쓰기 주체나 이중 transform 연산은 추적하지 않았으며 원인 확정으로 보고하지 않는다. 위치가 유지된 9개 초기 행만으로 성공을 선언하면 실패를 놓친다.

## 5. 검증 수준

최소 transform 계약 TESTED_FAILED, 미수용. 1개 runtime probe이며 모든 hierarchy 사용법 불가능 증명은 아니다. 성능 비교·8/64 회귀·시각 품질·최종 수용 미실행. guard가 실패를 감지해 소유 서버를 강제 종료했고 completedMarker=false/normalExit=false다. 실패 상태를 정상 퇴역으로 표시하지 않는다.

## 6. 변경 사항

새 격리 mission/runner/plan과 집계만 생성. 원본 소스·게임 PBO·운영 서버·개인키·기존 성공/실패 증거 보존. 실행 엔진 SHA256 4bbf370b1d479e0556e916d21213bafba383cbdd3a12c6e2518cd49c1ca64e70. runner의 scope 문자열은 이전 shadow 템플릿이 남아 부정확하므로 이번 시험 범위는 실제 plan/mission을 기준으로 판단한다. 종료 기록을 수정해 숨기지 않는다.

## 7. 원시 증거

host-parent-contract1(compile 실패), host-parent-contract1b(runtime 계약 실패)의 init.c·plan·launch·host-parent.tsv·분석 JSON을 사용했다. 전체 58행의 필드 정의 및 실패 행을 공개 evidence TXT에 투영한다. 로컬 경로·프로세스 식별자·원본 게임 자산·키·원시 전체 로그는 공개하지 않는다. 아래 SHA는 저장 전 파일 안정성 대조이며 옛 기록과 독립 재실행을 뜻하지 않는다.

## 8. 잔여 문제

부모 연결 후보를 8/64로 확대하지 않는다. 물리 제거·레이어 변경으로 이를 우회하는 방식도 이미 실패했으므로 자동 반복하지 않는다. 8→1 호스트 통합은 48본 도구 예산과 과거 표시 실패 경계를 아직 넘지 못했다. 물리 질의 broadphase는 직전 국소 비용 비교에서 기각됐다.

다음은 기존 활성/동결·부분 골격·1/8/64 비용 대조의 원래 조건과 비용 기여를 한 표로 연결해, 호스트 수 감소 없이 바꿀 수 있는 실행 작업이 있는지 결정하는 것이다. 비용 기여가 분리되지 않았는데 새 대규모 파이프라인을 만들지 않는다. 현재 전체 CPU 절감이나20ms p95 달성은 미입증이다. 별도 대량 표현 프록시 연구는 이 NPC 결과에 합치지 않는다.

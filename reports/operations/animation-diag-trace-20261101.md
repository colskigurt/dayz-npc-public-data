# 진단 mode2의 연속 호출·compressed pose 수집과 미확인 공간/alias

작성: 2026-11-01T17:33:35.429807+09:00

## 1. 작업 목표

정적 매핑한 A500 진단 함수·필드가 실제 mode2 개체에서 이어지는지 확인하고, 오래된 행렬 캐시와 구분되는 compressed pose를 수집한다. 수치 변경 없이 기존 NPC 파이프라인·일반 exe 시계 근거·64개체 기능/비용 연구를 보존한다.

## 2. 실제 수행

검증된 진단 client/server fixture를 물리1파트·고정run·루트/AI 이동OFF 조건으로1회 실행했다. 실제 서버 NPC READY 뒤에만 같은 profile·PID·실행 파일SHA를 대조해 Python Frida로 연결했다. 진단 모듈에 BEClient/BEServer/BattlEye 이름이 있으면 거부하고, 함수4개의 첫16바이트를 파일과 대조했다. 데이터 수치를 바꾸지는 않았지만 Interceptor hook과 injected 관찰 코드가 있으므로 무침습/무오버헤드가 아니며 성능 비교에서 제외한다.

진단 scheduler C6A020→dispatcher A7E7A0→consumer C680A0→interpolation C08340을 관찰했다. vtable·mode2·65본·포인터 shape 조건으로 대상1개를 고정했고 graphClock은 읽지 않았다. 매3번째 scheduler 종료에 두 skeleton의 이름6개에 대응하는32-byte transform을 기록했다. shader/renderer/GPU pose는 읽지 않았다. CU 활성화는 기록 종료 뒤로 미뤘지만 실제 시도 때 client가 이미 닫혀 화면을 확보하지 못했다. 실패를 보존한다.

## 3. 측정 결과

25.2109006초 유효 표본 범위,229표본, 네 함수 각689회, 계측 오류0·script 관찰 오류0을 확보했다. SDK 이름 해시→slot은 Head31/Hips3/spine2=13/Neck22/LeftHand29/RightHand64였다. capacity128, mode2,65본과 진단 vtable 조건이 실제로 맞았다. graphClock unavailable은 그대로다.

interpolation callback 간격 p50/p95/p99는36.737/38.503/42.076ms, 매3회 pose 표본 간격은110.089/114.822/120.542ms다. 이는 계측 callback/sample 간격이지 game frame-time이나 native self-time이 아니다. foreground/appActive를 연속 기록하지 않아 갱신 빈도의 원인을 특정하지 않는다. CU 활성화는 관찰 기간 중 요청하지 않았다.

retained duration은50.0000007ms였다. numerator/duration의 p50/p95/p99는0.577408/1.906318/2.0, 최대2.0이며689개 중67개가1을 넘었다.1초과 기록은 선택 후 약0.00055~22.93703초 범위에 있어 전부 시작 직후 현상이라고 단정할 수 없다. bufferCount 표본은0/1이다. 입력 비율이1을 넘는 것과 joint helper에서0..1로 clamp하는 것은 별개이며 실제 화면 정지 판정은 아니다.

Head 회전 성분은 인접228/228 표본에서 바뀌었다. 여섯 본을 포함한 query188/interpolated190 값은229/229 표본에서 정확히 같았다. skeleton/array pointer identity를 기록하지 않아 alias와 별도 저장소의 같은 결과를 구분하지 못한다.

서버 DONE·pose2151행·요청 상태2·최종commandSeconds44.9496/updates899다. 조기disconnect0·BE kick0·crash0, client 종료 marker·강제 종료 없음, 기존3개 PBO 서명 검증 성공이다. 기본 진단 플러그인 경고8줄은 남고 알려진 완결 경고로 구분했다. 새8/64 회귀·frame-time p50/p95/p99·GPU/display·할당/GC는 미측정이며20ms p95는 목표다.

## 4. 원인 분석

확인된 사실: 이 진단 환경에서는 native 관찰기를 붙인 채25초 이상 선택된 실제 mode2 경로와 pose 변화가 이어졌다. 기존 일반 exe의 BattlEye restart 이탈 문제를 해결했다고 소급하지 않는다. 진단1회 feasibility를 통과한 것이다.

확인된 사실: CPU storage 값은 변한다. 그러나 query와 interpolation 값의 일치는 저장소 alias일 수도 있어 현재 자료만으로 보간이 정상 작동한다거나 실패했다고 결론 내리지 않는다. 입력 비율1초과가 일부 시점에 있으므로 다음에는 queue/포인터/갱신 전후를 함께 봐야 한다.

기록 라벨 보정: 원시 JS가 word+0x1c를 scale로 명명했지만 의미는 검증되지 않았고 모든 표본이0이었다. 선정 파생본은 rawWord1C로만 제공한다. 세 positional component의 공간도 미확인이다. 예를 들어 첫 Head 성분 y약0.07575와 서버 API model-space Head y약1.60은 서로 다른 의미/시점일 수 있으므로 직접 MS 오차·지연을 계산하지 않는다. bone-local 후보지만 hierarchy나 정확한 API binding 확인 전 확정하지 않는다.

화면 관찰 실패로 CPU 변화와 실제 표시의 시간적 일치를 이번에 검증하지 못했다. 변경되는 본 값·callback 기록만으로 사용자가 본 낮은 프레임 문제가 해결됐다고 보고하지 않는다.

## 5. 검증 수준

진단 live object 선택·4단계 호출·25초 compressed pose 수집은 진단 성공이다.1회이며 반복 검증 완료는 아니다. 공간/alias/graphClock·실제 표시 연결은 미확인이다. 전체 NPC 기능 회귀, 일반 exe 해결, 시각 품질·성능 목표·최종 수용은 미완료다.

## 6. 변경 사항

새 격리 research의 launch·observer JS/Python·분석·선정 projection만 추가했다. 게임 코드·PBO·ANM·9350 후보·운영 서버는 변경하지 않았다.3개 PBO 기존 서명 검증은 성공했고 신규 PBO가 없어 새 서명 대상은 없다. hook 외의 게임 수치 쓰기/배율 보정·시계 패치·서비스/드라이버/보안 설정 변경은 없다. 새 영상·대용량dump·도구 설치는 없다.

## 7. 원시 증거

분석 ID animation-diag-trace-20261101-1730. runtime/native-observer.json·주입 JS·heartbeat·observer-finished·launch/profile·role-pose와 analyze.py를 보존한다. Python Frida이며 REA runtime Evidence가 아니다. 화면 저장 시도는 Unique diagnostic window absent로 실패했고 이미지가 없다. 이전 진단 기준 시험의 화면을 이번 실행 화면으로 대신 사용하지 않는다.

공개본은 집계와 처음/중간/마지막 표본3개의 필드 투영 및 한계다. 절대 프로세스 주소·PID/CLI·로컬 경로·사용자 식별자·전체 raw 로그·게임 자산·키는 제외한다. QPC tick은 동일 host의 시각 비교 좌표이며 UTC·영상시각과 자동 동기화된 것은 아니다. 종료·JSON·해시·파일 안정성 확인 뒤 기존 체크포인트를 저장했다.

## 8. 잔여 문제

다음은 skeleton/transform-array pointer equality와 정확한 GetBonePositionMS binding/좌표 공간을 확인하는 것이다. 확인 전 compressed 값과 SDK MS값을 직접 비교하지 않는다. 이후 foreground를 통제하고 기존 영상 인프라로 실제 표시와 동기화한 관찰을 설계한다. 단순히 새 보간/배율/업데이트 방식을 적용하지 않는다.

진단 조건과 일반 exe의 차이·계측 오버헤드를 유지하며 기존 mode0 손실·원본 변환 검증·64개체 회귀/비용을 별도 관리한다. 성능 목표는 이번 callback 통계로 판정하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `a2e4c4939e63e5df3cdf7fb6f594689f67de38abe0236e7d122a98fa486299c9`. 원시 전체가 아니다.
